#include "imgui/console.hpp"

#include "cmd.hpp"

#include <imgui.h>
#include <fmt/printf.h>
#include <misc/cpp/imgui_stdlib.h>


namespace imgui
{
    void Console::set_cmd_callback(Callback callback)
    {
        this->callback = callback;
    }

    void Console::push_cmd_result(const cmd::CommandResult& res)
    {
        log.push_back({
            .msg = res.msg,
            .color = res.state == cmd::CommandResultState::Ok ? 0xFFFFFFFF : 0xFF0000FF
        });
    }

    void Console::draw(bool* open)
    {
        if (!ImGui::Begin("Console", open))
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
            exec_cmd(command);
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

    void Console::exec_cmd(const std::string& cmd)
    {
        history.push_back(command);

        if (cmd == "clear")
        {
            log.clear();
        }
        else
        {
            log.push_back({ fmt::format(">> {:s}", command), 0xFFFFFFFF });
            if (callback == nullptr)
            {
                log.push_back({ fmt::format("unknown command {}", cmd), 0xFFFFFFFF });
            }
            else
            {
                callback(command);
            }
        }
    }

} // namespace imgui
