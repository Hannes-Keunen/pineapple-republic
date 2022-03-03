#include "renderer_stats.hpp"

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

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - timer).count() / 1000.0f;
        timer = now;

        ImGui::Text("frame time: %f (%f FPS)", delta, 1000.0f / delta);
        ImGui::Text("draw calls: %d", batch.get_draw_calls());

        ImGui::End();
    }

} // namespace imgui
