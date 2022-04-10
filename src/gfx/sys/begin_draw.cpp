#include "gfx/sys/systems.hpp"

#include "game_state.hpp"
#include "gfx/batch.hpp"
#include "gfx/camera.hpp"

namespace gfx::sys
{
    void begin_draw(GameState& state)
    {
        const auto& cam = state.get<gfx::Camera>();
        auto& batch = state.get<gfx::Batch>();
        batch.begin(cam.vp_matrix());
    }

} // namespace gfx::sys
