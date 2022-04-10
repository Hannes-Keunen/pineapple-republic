#include "registry.hpp"

#include "game_state.hpp"

#include <fmt/printf.h>

void init_registries(GameState& state, const InitData& data)
{
    auto& items = state.emplace<ItemRegistry>();
    for (const auto& item : data.items)
    {
        items.emplace(item.id, item);
    }
    fmt::print("[registry] loaded {} items\n", items.size());

    auto& crops = state.emplace<CropRegistry>();
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