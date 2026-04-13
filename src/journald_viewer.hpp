#pragma once
#include <string_view>

class JournalDViewer {
public:
    JournalDViewer();
    ~JournalDViewer() = default;

private:
    std::string_view color_for_priority(int p);
    std::string format_realtime_timestamp(std::string_view value);
};
