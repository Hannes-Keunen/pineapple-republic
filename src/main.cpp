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

std::atomic_bool running = true;

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

int main()
{
    entt::registry registry;
    cmd::init_handlers(registry);
    registry.ctx().emplace<TileMap>(10, 10);
    init_registries(registry, init());
    auto& cmd_queue = registry.ctx().emplace<TsQueue<std::string>>();

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
