#include "cmd.hpp"
#include "cmd/data.hpp"
#include "cmd/internal.hpp"
#include "game_state.hpp"

#include <entt/entt.hpp>

namespace cmd
{
    template <>
    auto parse<CmdExitData>(entt::registry& registry, const std::vector<std::string>& argv) -> std::optional<CmdExitData>
    {
        return CmdExitData {};
    }

    template <>
    auto exec<CmdExitData>(entt::registry& registry, const CmdExitData& data) -> CommandResult
    {
        registry.ctx().at<GameState>().this_thread().running = false;
        return result("received exit command, exiting...");
    }

} // namespace cmd
