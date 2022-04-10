#include "gfx/sys/systems.hpp"

#include "game_state.hpp"
#include "gfx/batch.hpp"

namespace gfx::sys
{
    void end_draw(GameState& state)
    {
        auto& batch = state.get<gfx::Batch>();
        batch.end();
    }

} // namespace gfx::sys
