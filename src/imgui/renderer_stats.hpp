#pragma once

#include "imgui/window.hpp"

#include <chrono>

namespace imgui
{
    class RendererStatsWindow : public Window
    {
    public:
        RendererStatsWindow();
        void draw(GameState& state, bool* visible) override;
    private:
        bool vsync = true;
        std::chrono::system_clock::time_point timer;
    };

} // namespace imgui
