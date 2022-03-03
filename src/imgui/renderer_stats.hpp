#pragma once

#include "gfx/batch.hpp"

#include <chrono>

namespace imgui
{
    class RendererStatsWindow
    {
    public:
        RendererStatsWindow();
        void draw(const gfx::Batch& batch);
    private:
        std::chrono::system_clock::time_point timer;
    };

} // namespace imgui
