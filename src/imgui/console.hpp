#pragma once

#include <functional>
#include <string>
#include <vector>

namespace imgui
{
    class Console
    {
    public:
        using Callback = std::function<void(const std::string&)>;
    public:
        void set_cmd_callback(Callback callback);
        void push_log_entry(const std::string& msg);
        void draw(bool* open);
    private:
        void exec_cmd(const std::string& cmd);
    private:
        std::string command;
        std::vector<std::string> history;
        std::vector<std::string> log;
        Callback callback;
        bool scroll_to_bottom;
    };

} // namespace imgui
