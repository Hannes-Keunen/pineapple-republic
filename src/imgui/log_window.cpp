#include "log_window.hpp"

#include "event.hpp"
#include "log.hpp"

#include <fmt/chrono.h>
#include <imgui.h>

namespace imgui
{
    LogWindow::LogWindow(GameState& state) : visible(true)
    {
        logger::subscribe([&, this](const logger::Entry& e)
        {
            logs.push_back(fmt::format("[{}][{}|{}] {}", e.timestamp, state.get_threads().at(e.thread_id).get_label(), e.level, e.msg));
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
