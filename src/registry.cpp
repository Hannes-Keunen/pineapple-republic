#include "registry.hpp"

#include <fmt/printf.h>

void init_registries(entt::registry &registry, const InitData& data)
{
    auto& items = registry.ctx().emplace<ItemRegistry>();
    for (const auto& item : data.items)
    {
        items.emplace(item.id, item);
    }
    fmt::print("[registry] loaded {} items\n", items.size());

    auto& crops = registry.ctx().emplace<CropRegistry>();
    for (const auto& crop : data.crops)
    {
        if (!items.contains(crop.produce_id))
        {
            fmt::print("[registry] skip crop {} with unknown produce_id {}\n", crop.id, crop.produce_id);
            continue;
        }

        crops.emplace(crop.id, crop);
    }
    fmt::print("[registry] loaded {} crops\n", crops.size());
}