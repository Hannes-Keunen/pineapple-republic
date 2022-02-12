#pragma once

#include <entt/entt.hpp>

namespace sys
{
    void growth_tick(entt::registry& registry, uint64_t delta);
} // namespace sys
