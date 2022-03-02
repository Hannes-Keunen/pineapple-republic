#include "game_state.hpp"

GameState::GameState()
{
    threads.try_emplace(std::this_thread::get_id(), "main", std::thread());
}

static void thread_func(GameState* state, entt::registry* registry, GameState::ThreadFunc f)
{
    f(*registry);
    state->this_thread().running = false;
}

void GameState::create_thread(std::string&& label, ThreadFunc&& f, entt::registry& registry)
{
    std::thread t(thread_func, this, &registry, f);
    threads.try_emplace(t.get_id(), std::forward<std::string>(label), std::move(t));
}
