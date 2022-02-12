#include "cmd.hpp"
#include "systems/systems.hpp"
#include "tsqueue.hpp"

#include <entt/entt.hpp>

#include <atomic>
#include <chrono>

void process_commands(entt::registry& registry)
{
    auto& cmd_queue = registry.ctx().at<TsQueue<std::string>>();
    while (!cmd_queue.is_empty())
    {
        cmd::parse_exec(registry, cmd_queue.pop());
    }
}

void update(entt::registry& registry, uint32_t delta)
{
    sys::growth_tick(registry, delta);
}

void simulation_main(entt::registry* registry)
{
    auto& running = registry->ctx().at<std::atomic_bool>();

    auto prev = std::chrono::system_clock::now();
    while(running)
    {
        process_commands(*registry);

        auto now = std::chrono::system_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - prev).count();
        prev = now;
        update(*registry, delta);
    }
}