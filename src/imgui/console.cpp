#include "imgui/console.hpp"

#include "cmd.hpp"
#include "game_state.hpp"

#include <imgui.h>
#include <fmt/printf.h>
#include <misc/cpp/imgui_stdlib.h>

namespace imgui
{
    void Console::draw(GameState& state, bool* visible)
    {
        auto& log_queue = state.get<TsQueue<cmd::CommandResult>>();
        while (!log_queue.is_empty())
        {
            auto res = log_queue.pop();
            log.push_back({
                .msg = res.msg,
                .color = res.state == cmd::CommandResultState::Ok ? 0xFFFFFFFF : 0xFF0000FF
            });
        }

        if (!ImGui::Begin("Console", visible))
        {
            ImGui::End();
            return;
        }

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow())
        {
            ImGui::EndPopup();
        }

        for (const auto& item : log)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, item.color);
            ImGui::TextUnformatted(item.msg.c_str());
            ImGui::PopStyleColor();
        }

        if (scroll_to_bottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        scroll_to_bottom = false;

        ImGui::EndChild();
        ImGui::Separator();

        bool reclaim_focus = false;
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText(">>", &command, flags))
        {
            exec_cmd(state, command);
            command.clear();
            reclaim_focus = true;
            scroll_to_bottom = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
        {
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }

        ImGui::End();
    }

    void Console::exec_cmd(GameState& state, const std::string& cmd)
    {
        history.push_back(command);

        if (cmd == "clear")
        {
            log.clear();
        }
        else
        {
            log.push_back({ fmt::format(">> {:s}", command), 0xFFFFFFFF });
            auto& cmd_queue = state.get<TsQueue<cmd::ConsoleCommand>>();
            cmd_queue.push({ cmd });
        }
    }

} // namespace imgui
