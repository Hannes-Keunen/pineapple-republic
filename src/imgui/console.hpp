#pragma once

#include "cmd.hpp"

#include <functional>
#include <string>
#include <vector>

namespace imgui
{
    class Console
    {
        struct Entry
        {
            std::string msg;
            uint32_t color;
        };
    public:
        using Callback = std::function<void(const std::string&)>;
    public:
        void set_cmd_callback(Callback callback);
        void push_cmd_result(const cmd::CommandResult& msg);
        void draw(bool* open);
    private:
        void exec_cmd(const std::string& cmd);
    private:
        std::string command;
        std::vector<std::string> history;
        std::vector<Entry> log;
        Callback callback;
        bool scroll_to_bottom;
    };

} // namespace imgui
