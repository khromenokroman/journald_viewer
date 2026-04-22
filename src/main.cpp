#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "journald_viewer.hpp"

int main() {
    try {
        JournalDViewer();

        return EXIT_SUCCESS;
    } catch (std::exception const &ex) {
        fmt::print(stderr, "Ошибка при работе программы: {}", ex.what());
        return EXIT_FAILURE;
    }
}
