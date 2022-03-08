#include "cmd.hpp"

#include "cmd/data.hpp"
#include "event.hpp"
#include "game_state.hpp"
#include "log.hpp"
#include "string.hpp"
#include "tsqueue.hpp"

#include <fmt/core.h>

#include <functional>
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
            auto& result_queue = registry.ctx().at<TsQueue<CommandResult>>();
            result_queue.push(result);

            auto& state = registry.ctx().at<GameState>();
            logger::i("{}", result.msg);
        }
    }

    void init_handlers(entt::registry& registry)
    {
        CommandMap commands;
        commands.emplace("plant", &handler_func<CmdPlantData>);
        commands.emplace("harvest", &handler_func<CmdHarvestData>);
        commands.emplace("exit", &handler_func<CmdExitData>);
        registry.ctx().emplace<CommandMap>(std::move(commands));

        registry.ctx().emplace<TsQueue<std::string>>();     // command queue
        registry.ctx().emplace<TsQueue<CommandResult>>();   // result queue
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
            auto& result_queue = registry.ctx().at<TsQueue<CommandResult>>();
            result_queue.push({ .msg = fmt::format("unknown command '{}'", argv[0]) });
            auto& state = registry.ctx().at<GameState>();
            logger::e("unknown command '{}'", argv[0]);
        }
    }

} // namespace cmd
