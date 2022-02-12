#include "systems/systems.hpp"

#include "model/crop.hpp"

#include <fmt/printf.h>

namespace sys
{
    void growth_tick(entt::registry& registry, uint64_t delta)
    {
        for (auto&& [entity, crop] : registry.view<Crop>().each())
        {
            crop.age += delta;
        }
    }

} // namespace sys
