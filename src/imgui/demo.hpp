#pragma once

#include "imgui/window.hpp"

namespace imgui
{
    class DemoWindow : public Window
    {
    public:
        void draw(GameState& state, bool* visible) override;
    };

} // namespace imgui
