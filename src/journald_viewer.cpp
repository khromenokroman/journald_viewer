#include "journald_viewer.hpp"

#include <fmt/format.h>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

JournalDViewer::JournalDViewer() {
    std::string reset = "\x1b[0m";
    std::string dim = "\x1b[90m";
    std::string cyan = "\x1b[36m";

    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            if (line.empty()) {
                continue;
            }

            ::nlohmann::json json;
            try {
                json = ::nlohmann::json::parse(line);
            } catch (const std::exception &ex) {
                fmt::print(stderr, "Ошибка парсинга JSON строки: {}\n", ex.what());
                throw;
            }

            auto ts_raw = json.at("__REALTIME_TIMESTAMP").get<std::string>();
            auto ts = format_realtime_timestamp(ts_raw);
            auto host = json.at("_HOSTNAME").get<std::string>();
            auto ident = json.value("_COMM", "unknown");
            auto pid = json.value("_PID", "unknown");
            auto msg = json.at("MESSAGE").get<std::string>();
            auto prio = std::stoi(json.value("PRIORITY", "6"));

            auto msg_color = color_for_priority(prio);

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
        } catch (std::exception &ex) {
            fmt::print(stderr, "Ошибка при разборе строки:\n{}\n{}\n", ::nlohmann::json::parse(line).dump(2),
                       ex.what());
            throw;
        }
    }
}
std::string_view JournalDViewer::color_for_priority(int p) {
    switch (p) {
        case 0:
        case 1:
        case 2:
            return "\x1b[1;31m"; // emergency/alert/crit
        case 3:
            return "\x1b[31m"; // error
        case 4:
            return "\x1b[1;33m"; // warning
        case 5:
            return "\x1b[1;37m"; // notice
        case 6:
            return "\x1b[37m"; // info
        case 7:
            return "\x1b[90m"; // debug
        default:
            return "\x1b[0m";
    }
}
std::string JournalDViewer::format_realtime_timestamp(std::string_view value) {
    if (value.empty()) {
        throw std::runtime_error("Empty value date/time");
    }

    auto const us = std::stoll(value.data());
    auto const sec = static_cast<std::time_t>(us / 1000000);
    auto const micros = static_cast<int>(us % 1000000);

    std::tm tm{};
    localtime_r(&sec, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(6) << std::setfill('0') << micros;
    return oss.str();
}
