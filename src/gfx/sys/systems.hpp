#pragma once

class GameState;

namespace gfx::sys
{
    void cam_control(GameState& state);

    /**
     * AFTER: cam_control
     */
    void begin_draw(GameState& state);

    /**
     * AFTER: begin_draw
     * BEFORE: end_draw
     */
    void draw_map(GameState& state);

    void end_draw(GameState& state);
} // namespace gfx::sys
