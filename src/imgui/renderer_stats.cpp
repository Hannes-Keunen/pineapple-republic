#include "renderer_stats.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace imgui
{
    RendererStatsWindow::RendererStatsWindow()
    {
        timer = std::chrono::system_clock::now();
    }

    void RendererStatsWindow::draw(const gfx::Batch& batch)
    {
        if (!ImGui::Begin("Renderer"))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Checkbox("Enable Vsync", &vsync))
        {
            glfwSwapInterval(vsync ? 1 : 0);
        }

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - timer).count() / 1000.0f;
        timer = now;
        ImGui::Text("frame time: %f (%f FPS)", delta, 1000.0f / delta);

        auto stats = batch.get_stats();
        ImGui::Text("# sprites: %d", stats.sprites);
        ImGui::Text("# draw calls: %d", stats.draws);
        ImGui::Text("# textures: %d", stats.textures);

        ImGui::End();
    }

} // namespace imgui
