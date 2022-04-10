#include "imgui/window.hpp"

namespace imgui
{
    void WindowManager::toggle_window(int key)
    {
        if (windows.contains(key))
        {
            auto& w = windows.at(key);
            w.second = !w.second;
        }
    }

    void draw_windows(GameState& state)
    {
        auto& manager = state.get<WindowManager>();
        for (auto&& [_, window] : manager.get_windows())
        {
            if (window.second)
            {
                window.first->draw(state, &window.second);
            }
        }
    }

} // namespace imgui
