#include "cmd.hpp"
#include "event.hpp"
#include "game_state.hpp"
#include "log.hpp"
#include "model/crop.hpp"
#include "model/item.hpp"
#include "registry.hpp"
#include "threading.hpp"
#include "tilemap.hpp"

#include <entt/entt.hpp>
#include <fmt/chrono.h>

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

void simulation_main(GameState& state);
void graphics_main(GameState& state);

int main()
{
    GameState state;
    cmd::init_handlers(state);

    auto& tc = state.emplace<ThreadConfig>();

    logger::subscribe([&](const logger::Entry& e)
    {
        fmt::print("[{}][{}|{}] {}\n", e.timestamp, tc.get_threads().at(e.thread_id).get_label(), e.level, e.msg);
    });

    tc.create_thread("simulation", simulation_main, state);
    tc.create_thread("graphics", graphics_main, state);

    // TODO: move to simulation thread
    state.emplace<TileMap>(10, 10);
    init_registries(state, init());

    bool running = true;
    while (running)
    {
        for (const auto& [_, thread] : tc.get_threads())
        {
            if (!thread.running)
            {
                logger::info("thread {} has stopped, exiting...", thread.get_label());
                running = false;
            }
        }

        logger::drain();
    }

    for (auto& [_, data] : tc.get_threads()) { data.running = false; }
    for (auto& [_, data] : tc.get_threads()) { if (data.thread.joinable()) { data.thread.join(); } }
}
