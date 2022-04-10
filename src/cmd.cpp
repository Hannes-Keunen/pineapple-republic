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
    using CommandHandler = std::function<void(GameState&, const std::vector<std::string>&)>;
    using CommandMap = std::unordered_map<std::string, CommandHandler>;

    template <typename T> void handler_func(GameState& state, const std::vector<std::string>& argv)
    {
        auto data = parse<T>(state, argv);
        if (data != std::nullopt)
        {
            auto result = exec(state, data.value());
            auto& result_queue = state.get<TsQueue<CommandResult>>();
            result_queue.push(result);

            switch (result.state)
            {
            case CommandResultState::Ok: logger::info("{}", result.msg);
            case CommandResultState::Error: logger::error("{}", result.msg);
            }
            
        }
    }

    void init_handlers(GameState& state)
    {
        CommandMap commands;
        commands.emplace("plant", &handler_func<CmdPlantData>);
        commands.emplace("harvest", &handler_func<CmdHarvestData>);
        commands.emplace("exit", &handler_func<CmdExitData>);
        state.emplace<CommandMap>(std::move(commands));

        state.emplace<TsQueue<ConsoleCommand>>();  // command queue
        state.emplace<TsQueue<CommandResult>>();   // result queue
    }

    void parse_exec(GameState& state, const ConsoleCommand& cmd)
    {
        auto argv = str::split(cmd.cmd);
        auto& commands = state.get<CommandMap>();
        if (commands.contains(argv[0]))
        {
            commands.at(argv[0])(state, argv);
        }
        else
        {
            auto& result_queue = state.get<TsQueue<CommandResult>>();
            result_queue.push({ .msg = fmt::format("unknown command '{}'", argv[0]) });
            logger::error("unknown command '{}'", argv[0]);
        }
    }

} // namespace cmd
