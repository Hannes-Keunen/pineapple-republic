#include "cmd.hpp"
#include "cmd/data.hpp"
#include "cmd/internal.hpp"
#include "tilemap.hpp"
#include "model/crop.hpp"
#include "registry.hpp"
#include "string.hpp"

#include <entt/entt.hpp>
#include <fmt/core.h>

#include <optional>
#include <string>
#include <vector>

namespace cmd
{
    template <>
    auto parse<CmdPlantData>(GameState& state, const std::vector<std::string>& argv) -> std::optional<CmdPlantData>
    {
        if (!check_argc(argv.size(), 4))
        {
            return std::nullopt;
        }

        CmdPlantData result;
        result.plant_id = argv[1];
        if (!parse_int_argument(argv[2], result.x)) { return std::nullopt; }
        if (!parse_int_argument(argv[3], result.y)) { return std::nullopt; }
        return result;
    }

    template <>
    auto exec<CmdPlantData>(GameState& state, const CmdPlantData& data) -> CommandResult
    {
        auto& map = state.get<TileMap>();
        if (data.x > map.get_width() || data.y > map.get_height())
        {
            return error_result("tile index out of range: ({},{}), range is ({},{}", data.x, data.y, map.get_width(), map.get_height());
        }

        if (!state.get<CropRegistry>().contains(data.plant_id))
        {
            return error_result("unknown crop {}", data.plant_id);
        }
        auto &tile = map.tile_at(data.x, data.y);

        auto& ecs = state.get_ecs();
        auto entity = ecs.create();
        ecs.emplace<Crop>(entity, data.plant_id, 0ul);
        tile.entity = entity;

        return success_result("planted {} at ({},{})", data.plant_id, data.x, data.y);
    }

} // namespace cmd
