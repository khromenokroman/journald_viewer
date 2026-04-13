#include <fstream>
#include <iostream>
#include <string>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static std::string get_string(const json& j, const char* key, const std::string& fallback = "") {
    if (!j.contains(key) || j.at(key).is_null()) {
        return fallback;
    }

    const auto& v = j.at(key);
    if (v.is_string()) {
        return v.get<std::string>();
    }

    if (v.is_number_integer()) {
        return std::to_string(v.get<long long>());
    }

    if (v.is_array()) {
        std::string out;
        bool first = true;
        for (const auto& item : v) {
            if (!first) {
                out += ", ";
            }
            first = false;
            if (item.is_string()) {
                out += item.get<std::string>();
            } else {
                out += item.dump();
            }
        }
        return out;
    }

    return v.dump();
}

static const char* color_for_priority(const std::string& p) {
    if (p == "0" || p == "1" || p == "2") return "\x1b[1;31m"; // emergency/alert/crit
    if (p == "3") return "\x1b[31m";                            // error
    if (p == "4") return "\x1b[33m";                            // warning
    if (p == "5") return "\x1b[1;37m";                          // notice
    if (p == "6") return "\x1b[37m";                            // info
    if (p == "7") return "\x1b[90m";                            // debug
    return "\x1b[0m";
}

static std::string make_json_array_text(const std::string& text) {
    std::string out;
    out.reserve(text.size() + 2);

    out.push_back('[');

    bool in_string = false;
    bool escape = false;
    int depth = 0;
    bool need_comma = false;

    for (size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];

        if (in_string) {
            out.push_back(c);
            if (escape) {
                escape = false;
            } else if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }

        if (c == '"') {
            in_string = true;
            out.push_back(c);
            continue;
        }

        if (c == '{') {
            if (need_comma) {
                out.push_back(',');
            }
            out.push_back(c);
            ++depth;
            need_comma = false;
            continue;
        }

        if (c == '}') {
            out.push_back(c);
            --depth;
            if (depth == 0) {
                need_comma = true;
            }
            continue;
        }

        if (depth == 0) {
            if (c == '{' || c == '}' || c == ',' || c == '[' || c == ']') {
                continue;
            }
            if (std::isspace(static_cast<unsigned char>(c))) {
                continue;
            }
        }

        out.push_back(c);
    }

    out.push_back(']');
    return out;
}

int main(int argc, char* argv[]) {
    const std::string path = (argc > 1) ? argv[1] : "journal.json";

    std::ifstream in(path);
    if (!in) {
        fmt::print(stderr, "Не удалось открыть файл: {}\n", path);
        return 1;
    }

    const std::string raw((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const std::string wrapped = make_json_array_text(raw);

    json root;
    try {
        root = json::parse(wrapped);
    } catch (const std::exception& e) {
        fmt::print(stderr, "Ошибка парсинга JSON: {}\n", e.what());
        return 1;
    }

    if (!root.is_array()) {
        fmt::print(stderr, "Ожидался JSON-массив объектов\n");
        return 1;
    }

    const std::string reset = "\x1b[0m";
    const std::string dim = "\x1b[90m";
    const std::string cyan = "\x1b[36m";

    for (const auto& e : root) {
        const std::string ts = get_string(e, "SYSLOG_TIMESTAMP", get_string(e, "__REALTIME_TIMESTAMP"));
        const std::string host = get_string(e, "_HOSTNAME");
        const std::string ident = get_string(e, "SYSLOG_IDENTIFIER", get_string(e, "_COMM"));
        const std::string pid = get_string(e, "SYSLOG_PID", get_string(e, "_PID"));
        const std::string msg = get_string(e, "MESSAGE");
        const std::string prio = get_string(e, "PRIORITY", "6");

        const char* msg_color = color_for_priority(prio);

        if (!ts.empty()) {
            fmt::print("{}{}{} ", dim, ts, reset);
        }

        if (!host.empty()) {
            fmt::print("{}{}{} ", dim, host, reset);
        }

        if (!ident.empty()) {
            if (!pid.empty()) {
                fmt::print("{}{}[{}]{}: ", cyan, ident, pid, reset);
            } else {
                fmt::print("{}{}{}: ", cyan, ident, reset);
            }
        }

        fmt::print("{}{}{}\n", msg_color, msg, reset);
    }

    return 0;
}