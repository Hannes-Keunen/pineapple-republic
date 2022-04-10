#include "gfx/sys/systems.hpp"

#include "game_state.hpp"
#include "gfx/batch.hpp"
#include "gfx/camera.hpp"
#include "res/cache.hpp"
#include "tilemap.hpp"

namespace gfx::sys
{
    constexpr static float TILE_SIZE = 1.0f;

    void draw_map(GameState& state)
    {
        const auto& map = state.get<TileMap>();
        auto& batch = state.get<Batch>();
        auto& textures = state.get<res::Cache<gfx::gl::Texture>>();
        for (int y = 0; y < map.get_height(); ++y)
        {
            for (int x = 0; x < map.get_width(); ++x)
            {
                const auto& tile = map.tile_at(x, y);
                const auto& texture = textures.get("../res/textures/farm_tile.bmp");
                batch.submit(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, texture);
            }
        }
    }
} // namespace gfx::sys
