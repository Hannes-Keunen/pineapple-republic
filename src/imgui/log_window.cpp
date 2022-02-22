#include "log_window.hpp"

#include "event.hpp"
#include "log.hpp"

#include <fmt/chrono.h>
#include <imgui.h>

namespace imgui
{
    LogWindow::LogWindow(GameState& state) : visible(true)
    {
        state.on_event<logging::Entry>([this](const logging::Entry& e)
        {
            logs.push_back(fmt::format("[{}][{}|{}] {}", e.timestamp, e.thread_id, e.level, e.msg));
        });
    }

    void LogWindow::draw()
    {
        if (!ImGui::Begin("Log", &visible))
        {
            ImGui::End();
            return;
        }

        for (const auto& msg : logs)
        {
            ImGui::TextUnformatted(msg.c_str());
        }

        ImGui::End();
    }

} // namespace imgui
