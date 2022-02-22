#pragma once

#include "game_state.hpp"

#include <string>
#include <vector>

namespace imgui
{
    class LogWindow
    {
    public:
        LogWindow(GameState& state);
        void draw();
    private:
        bool visible;
        std::vector<std::string> logs;
    };

} // namespace imgui
