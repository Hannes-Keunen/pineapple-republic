#include "imgui/demo.hpp"

#include "imgui.h"

namespace imgui
{
    void DemoWindow::draw(GameState& state, bool* visible)
    {
        ImGui::ShowDemoWindow(visible);
    }

} // namespace imgui
