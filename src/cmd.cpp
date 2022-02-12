#include "cmd.hpp"

#include "cmd/data.hpp"
#include "string.hpp"

#include <fmt/printf.h>

#include <functional>
#include <iostream>
#include <unordered_map>

namespace cmd
{
    using CommandHandler = std::function<void(entt::registry&, const std::vector<std::string>&)>;
    using CommandMap = std::unordered_map<std::string, CommandHandler>;

    template <typename T> void handler_func(entt::registry& registry, const std::vector<std::string>& argv)
    {
        auto data = parse<T>(registry, argv);
        if (data != std::nullopt)
        {
            auto result = exec(registry, data.value());
            fmt::print("[cmd:{}] {}\n", argv[0], result.msg);
        }
    }

    void init_handlers(entt::registry& registry)
    {
        CommandMap commands;
        commands.emplace("plant", &handler_func<CmdPlantData>);
        commands.emplace("harvest", &handler_func<CmdHarvestData>);
        registry.ctx().emplace<CommandMap>(std::move(commands));
    }

    void parse_exec(entt::registry& registry, const std::string& cmd)
    {
        auto argv = str::split(cmd);
        auto& commands = registry.ctx().at<CommandMap>();
        if (commands.contains(argv[0]))
        {
            commands.at(argv[0])(registry, argv);
        }
        else
        {
            fmt::print("[cmd] unknown command '{}'\n", argv[0]);
        }
    }

} // namespace cmd
