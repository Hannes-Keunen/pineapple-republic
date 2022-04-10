#include "threading.hpp"

ThreadConfig::ThreadConfig()
{
    threads.try_emplace(std::this_thread::get_id(), "main", std::thread());
}

static void thread_func(ThreadConfig* tc, GameState* state, ThreadConfig::ThreadFunc f)
{
    f(*state);
    tc->this_thread().running = false;
}

void ThreadConfig::create_thread(std::string&& label, ThreadFunc&& f, GameState& state)
{
    std::thread t(thread_func, this, &state, f);
    threads.try_emplace(t.get_id(), std::forward<std::string>(label), std::move(t));
}
