#include "cmd.hpp"
#include "event.hpp"
#include "game_state.hpp"
#include "log.hpp"
#include "model/crop.hpp"
#include "model/item.hpp"
#include "registry.hpp"
#include "systems/systems.hpp"
#include "tilemap.hpp"
#include "tsqueue.hpp"

#include <entt/entt.hpp>
#include <fmt/chrono.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

auto init() -> InitData
{
    return {
        .items = {
            { "item:pineapple", "items/pineapple.bmp", "pineapple" }
        },
        .crops = {
            { "crop:pineapple", "crops/pineapple.bmp", "pineapple", "item:pineapple", 500000 }
        }
    };
}

void simulation_main(entt::registry& registry);
void graphics_main(entt::registry& registry);

int main()
{
    entt::registry registry;
    cmd::init_handlers(registry);
    registry.ctx().emplace<TileMap>(10, 10);
    init_registries(registry, init());

    auto& state = registry.ctx().emplace<GameState>();
    state.create_thread("simulation", simulation_main, registry);
    state.create_thread("graphics", graphics_main, registry);
    state.on_event<logging::Entry>([](const logging::Entry& e)
    {
        fmt::print("[{}][{}|{}] {}\n", e.timestamp, e.thread_id, e.level, e.msg);
    });

    bool running = true;
    while (running)
    {
        for (const auto& [_, thread] : state.get_threads())
        {
            if (!thread.running)
            {
                state.log_i("thread {} has stopped, exiting...", thread.get_label());
                running = false;
            }
        }

        state.get<EventBus<logging::Entry>>().drain();
    }

    for (auto& [_, data] : state.get_threads()) { data.running = false; }
    for (auto& [_, data] : state.get_threads()) { if (data.thread.joinable()) { data.thread.join(); } }
}
