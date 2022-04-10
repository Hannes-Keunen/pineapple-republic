#pragma once

#include "game_state.hpp"

#include <optional>
#include <string>
#include <vector>

namespace cmd
{
    enum class CommandResultState
    {
        Ok,
        Error
    };

    struct CommandResult
    {
        CommandResultState state;
        std::string msg;
    };

    struct ConsoleCommand
    {
        std::string cmd;
    };

    template <typename T> auto parse(GameState& state, const std::vector<std::string>& argv) -> std::optional<T>;
    template <typename T> auto exec(GameState& state, const T& data) -> CommandResult;

    void init_handlers(GameState& state);
    void parse_exec(GameState& state, const ConsoleCommand& cmd);

} // namespace cmd
