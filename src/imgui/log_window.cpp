#include "log_window.hpp"

#include "event.hpp"
#include "game_state.hpp"
#include "log.hpp"
#include "threading.hpp"

#include <fmt/chrono.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace imgui
{
    LogWindow::LogWindow(GameState& state) : visible(true)
    {
        logger::subscribe([&, this](const logger::Entry& e)
        {
            uint32_t color = 0xFFFFFFFF;
            switch (e.level) {
            case logger::Level::Warning: color = 0xFFFF00FF;
            case logger::Level::Error: color = 0xFF0000FF;
            }

            std::string thread = state.get<ThreadConfig>().get_threads().at(e.thread_id).get_label();
            std::string level = fmt::format("{}", e.level);

            logs.push_back({
                .timestamp = fmt::format("{}", e.timestamp),
                .level = level,
                .thread_id = thread,
                .msg = e.msg,
                .color = color
            });

            thread_ids.emplace(thread);
            levels.emplace(level);
        });
    }

    void LogWindow::draw()
    {
        if (!ImGui::Begin("Log", &visible))
        {
            ImGui::End();
            return;
        }

        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2);
        ImGui::TextUnformatted("thread:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##thread", thread_filter == std::nullopt ? "<all>" : thread_filter->c_str()))
        {
            if (ImGui::Selectable("<all>"))
            {
                thread_filter = std::nullopt;
            }
            for (const auto& tid : thread_ids)
            {
                if (ImGui::Selectable(tid.c_str()))
                {
                    thread_filter = tid;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::TextUnformatted("level:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##level", level_filter == std::nullopt ? "<all>" : level_filter->c_str()))
        {
            if (ImGui::Selectable("<all>"))
            {
                level_filter = std::nullopt;
            }
            for (const auto& level : levels)
            {
                if (ImGui::Selectable(level.c_str()))
                {
                    level_filter = level;
                }
            }
            ImGui::EndCombo();
        }


        ImGui::SameLine();
        ImGui::TextUnformatted("message (regex):");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::InputText("##message", &msg_filter))
        {
            try
            {
                msg_filter_regex.assign(msg_filter);
            }
            catch (std::regex_error& e) { }
        }


        ImGui::BeginTable("logs", 4, ImGuiTableFlags_Resizable);
        for (const auto& entry : logs)
        {
            if (thread_filter != std::nullopt && thread_filter.value() != entry.thread_id)
            {
                continue;
            }
            if (level_filter != std::nullopt && level_filter.value() != entry.level)
            {
                continue;
            }
            if (!msg_filter.empty() && !std::regex_search(entry.msg, msg_filter_regex))
            {
                continue;
            }

            ImGui::TableNextRow();
            ImGui::PushStyleColor(ImGuiCol_Text, entry.color);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entry.timestamp.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entry.thread_id.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entry.level.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(entry.msg.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::EndTable();

        ImGui::End();
    }

} // namespace imgui
