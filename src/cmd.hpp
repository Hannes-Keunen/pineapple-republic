#pragma once

#include <entt/entt.hpp>

#include <optional>
#include <string>
#include <vector>

namespace cmd
{
    struct CommandResult
    {
        std::string msg;
    };

    template <typename T> auto parse(entt::registry& registry, const std::vector<std::string>& argv) -> std::optional<T>;
    template <typename T> auto exec(entt::registry& registry, const T& data) -> CommandResult;

    void init_handlers(entt::registry& registry);
    void parse_exec(entt::registry& registry, const std::string& cmd);

} // namespace cmd
