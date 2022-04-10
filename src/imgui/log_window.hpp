#pragma once

#include "imgui/window.hpp"

#include <regex>
#include <set>
#include <string>
#include <vector>

namespace imgui
{
    class LogWindow : public Window
    {
        struct Entry
        {
            std::string timestamp;
            std::string level;
            std::string thread_id;
            std::string msg;
            uint32_t color;
        };
    public:
        LogWindow(GameState& state);
        void draw(GameState& state, bool* visible) override;
    private:
        std::vector<Entry> logs;
        std::set<std::string> thread_ids;
        std::set<std::string> levels;

        std::optional<std::string> thread_filter;
        std::optional<std::string> level_filter;
        std::string msg_filter;
        std::regex msg_filter_regex;
    };

} // namespace imgui
