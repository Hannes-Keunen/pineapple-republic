#include "cmd.hpp"
#include "game_state.hpp"
#include "systems/systems.hpp"
#include "threading.hpp"
#include "tsqueue.hpp"

#include <atomic>
#include <chrono>

void process_commands(GameState& state)
{
    auto& cmd_queue = state.get<TsQueue<cmd::ConsoleCommand>>();
    while (!cmd_queue.is_empty())
    {
        cmd::parse_exec(state, cmd_queue.pop());
    }
}

void update(GameState& state, uint32_t delta)
{
    sys::growth_tick(state.get_ecs(), delta);
}

void simulation_main(GameState& state)
{
    auto prev = std::chrono::system_clock::now();
    auto& tc = state.get<ThreadConfig>();
    while(tc.this_thread().running)
    {
        process_commands(state);

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - prev).count();
        prev = now;
        update(state, delta);
    }
}
