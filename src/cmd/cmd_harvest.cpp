#include "cmd.hpp"
#include "cmd/data.hpp"
#include "cmd/internal.hpp"
#include "registry.hpp"
#include "tilemap.hpp"

#include <entt/entt.hpp>
#include <fmt/core.h>

#include <optional>
#include <string>
#include <vector>

namespace cmd
{
    template <>
    auto parse<CmdHarvestData>(entt::registry& registry, const std::vector<std::string>& argv) -> std::optional<CmdHarvestData>
    {
        if (!check_argc(argv.size(), 3))
        {
            return std::nullopt;
        }

        CmdHarvestData data;
        if (!parse_int_argument(argv[1], data.x)) { return std::nullopt; }
        if (!parse_int_argument(argv[2], data.y)) { return std::nullopt; }
        return data;
    }

    template <>
    auto exec<CmdHarvestData>(entt::registry& registry, const CmdHarvestData& data) -> CommandResult
    {
        auto& map = registry.ctx().at<TileMap>();
        if (data.x > map.get_width() || data.y > map.get_height())
        {
            return error_result("tile index out of range: ({},{}), range is ({},{}", data.x, data.y, map.get_width(), map.get_height());
        }

        auto& tile = map.tile_at(data.x, data.y);
        if (tile.entity == entt::null)
        {
            return error_result("there is no crop at ({},{})", data.x, data.y);
        }

        auto& crop = registry.get<Crop>(tile.entity);
        auto& type = registry.ctx().at<CropRegistry>().at(crop.type);
        if (crop.age < type.grow_time)
        {
            return error_result("crop {} at tile ({},{}) is not ripe yet ({}/{})", crop.type, data.x, data.y, crop.age, type.grow_time);
        }

        tile.entity = entt::null;
        // TODO: add crop harvest item to the harvester's inventory
        return success_result("harvested crop {} at tile ({},{}): {}", crop.type, data.x, data.y, type.produce_id);
    }

} // namespace cmd
