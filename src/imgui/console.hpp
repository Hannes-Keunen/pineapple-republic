#pragma once

#include "cmd.hpp"
#include "imgui/window.hpp"

#include <functional>
#include <string>
#include <vector>

namespace imgui
{
    class Console : public Window
    {
        struct Entry
        {
            std::string msg;
            uint32_t color;
        };
    public:
        void draw(GameState& state, bool* visible) override;
    private:
        void exec_cmd(GameState& state, const std::string& cmd);
    private:
        std::string command;
        std::vector<std::string> history;
        std::vector<Entry> log;
        bool scroll_to_bottom;
    };

} // namespace imgui
