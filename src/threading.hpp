#pragma once

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>

class ThreadData
{
public:
    std::atomic_bool running;
    std::thread thread;
public:
    ThreadData(std::string&& label, std::thread&& thread)
        : label(label), thread(std::forward<std::thread>(thread)), running(true) {}
    ThreadData(const ThreadData&) = delete;
    ThreadData(ThreadData&&) = default;

    constexpr const auto& get_label() const { return label; }
private:
    std::string label;
};

class GameState;

class ThreadConfig
{
public:
    ThreadConfig();
    ThreadConfig(const ThreadConfig&) = delete;
    ThreadConfig(ThreadConfig&&) = default;
public:
    using ThreadFunc = std::function<void(GameState& state)>;
    void create_thread(std::string&& label, ThreadFunc&& f, GameState& state);

    auto& get_threads() const { return threads; }
    auto& get_threads() { return threads; }
    auto& this_thread() const { return threads.at(std::this_thread::get_id()); }
    auto& this_thread() { return threads.at(std::this_thread::get_id()); }
private:
    std::unordered_map<std::thread::id, ThreadData> threads;
};
