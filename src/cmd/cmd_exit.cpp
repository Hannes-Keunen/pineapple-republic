#include "cmd.hpp"
#include "cmd/data.hpp"
#include "cmd/internal.hpp"
#include "game_state.hpp"
#include "threading.hpp"

#include <entt/entt.hpp>

namespace cmd
{
    template <>
    auto parse<CmdExitData>(GameState& state, const std::vector<std::string>& argv) -> std::optional<CmdExitData>
    {
        return CmdExitData {};
    }

    template <>
    auto exec<CmdExitData>(GameState& state, const CmdExitData& data) -> CommandResult
    {
        state.get<ThreadConfig>().this_thread().running = false;
        return success_result("received exit command, exiting...");
    }

} // namespace cmd
