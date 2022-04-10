#pragma once

#include <memory>
#include <unordered_map>
#include <utility>

#include "game_state.hpp"

namespace imgui
{
    class Window
    {
    public:
        virtual void draw(GameState& state, bool* visible) = 0;
    };

    class WindowManager
    {
    public:
        template <typename T, typename... Args>
        void add_window(int key, Args&&... args)
        {
            windows.try_emplace(key, std::make_unique<T>(std::forward<Args>(args)...), false);
        }

        constexpr auto& get_windows() { return windows; }

        void toggle_window(int key);

    private:
        std::unordered_map<int, std::pair<std::unique_ptr<Window>, bool>> windows;
    };

    void draw_windows(GameState& state);

} // namespace imgui
