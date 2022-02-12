#include "cmd.hpp"
#include "model/crop.hpp"
#include "model/item.hpp"
#include "registry.hpp"
#include "systems/systems.hpp"
#include "tilemap.hpp"
#include "tsqueue.hpp"

#include <entt/entt.hpp>
#include <fmt/printf.h>

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

void simulation_main(entt::registry* registry);

int main()
{
    entt::registry registry;
    cmd::init_handlers(registry);
    registry.ctx().emplace<TileMap>(10, 10);
    init_registries(registry, init());
    auto& cmd_queue = registry.ctx().emplace<TsQueue<std::string>>();
    auto& running = registry.ctx().emplace<std::atomic_bool>();

    std::thread simulation_thread(simulation_main, &registry);

    while (running)
    {
        std::string command;
        std::cout << ">> ";
        std::getline(std::cin, command);
        if (command == "exit")
        {
            running = false;
        }
        else
        {
            cmd_queue.push(command);
        }
    }

    simulation_thread.join();
}
