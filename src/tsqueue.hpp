#pragma once

#include <mutex>
#include <optional>
#include <queue>

// Thread-safe queue
template <typename T>
class TsQueue
{
public:
    TsQueue() = default;
    TsQueue(const TsQueue&) = delete;
    TsQueue(TsQueue&&) = default;
    TsQueue& operator=(const TsQueue&) = delete;
    TsQueue& operator=(TsQueue&&) = default;

    void push(const T& t)
    {
        std::lock_guard<std::mutex> lock(mutex);
        q.push(t);
    }

    void push(T&& t)
    {
        std::lock_guard<std::mutex> lock(mutex);
        q.push(t);
    }

    auto pop() -> T
    {
        std::lock_guard<std::mutex> lock(mutex);
        T t = q.front();
        q.pop();
        return t;
    }

    auto is_empty() const -> bool
    {
        std::lock_guard<std::mutex> lock(mutex);
        return q.empty();
    }

private:
    std::queue<T> q;
    mutable std::mutex mutex;
};
