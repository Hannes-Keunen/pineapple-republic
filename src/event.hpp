#pragma once

#include "tsqueue.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename T>
class EventBus
{
public:
    using Callback = std::function<void(const T&)>;
public:
    void publish(const T& t)
    {
        for (auto& [_, q] : queues)
        {
            q.push(t);
        }
    }

    void drain()
    {
        if (!queues.contains(std::this_thread::get_id()) || !queues.contains(std::this_thread::get_id()))
        {
            return;
        }

        auto& queue = queues.at(std::this_thread::get_id());
        auto& handlers = callbacks.at(std::this_thread::get_id());
        while (!queue.is_empty())
        {
            T event = queue.pop();
            for (auto& handler : handlers)
            {
                handler(event);
            }
        }
    }

    void subscribe(std::function<void(const T&)> callback)
    {
        std::lock_guard<std::mutex> q_lock(queues_mutex);
        std::lock_guard<std::mutex> c_lock(callbacks_mutex);
        queues.try_emplace(std::this_thread::get_id());
        callbacks[std::this_thread::get_id()].push_back(callback);
    }

private:
    std::atomic_bool dummy;
    std::unordered_map<std::thread::id, std::vector<Callback>> callbacks;
    std::unordered_map<std::thread::id, TsQueue<T>> queues;
    std::mutex queues_mutex, callbacks_mutex;
};
