#pragma once

#include "any.hpp"
#include "event.hpp"
#include "log.hpp"

#include <entt/entt.hpp>

#include <any>
#include <atomic>
#include <string>
#include <thread>
#include <typeindex>
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

class GameState
{
public:
    template <typename T>
    T& get()
    {
        return ctx.at(std::type_index(typeid(T))).as<T>();
    }

    template <typename T, typename... Args>
    T& emplace(Args&&... args)
    {
        ctx.try_emplace(std::type_index(typeid(T)), make_any<T>(std::forward<Args>(args)...));
        return ctx.at(std::type_index(typeid(T))).as<T>();
    }

    template <typename T>
    void publish_event(const T& event)
    {
        EventBus<T>& bus = emplace<EventBus<T>>();
        bus.publish(event);
    }

    template <typename T>
    void on_event(EventBus<T>::Callback callback)
    {
        EventBus<T>& bus = emplace<EventBus<T>>();
        bus.subscribe(callback);
    }

public:
    template <typename... T>
    void log_t(fmt::format_string<T...> fmt, T&&... args)
    {
        log(logging::Level::Trace, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void log_d(fmt::format_string<T...> fmt, T&&... args)
    {
        log(logging::Level::Debug, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void log_i(fmt::format_string<T...> fmt, T&&... args)
    {
        log(logging::Level::Info, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void log_w(fmt::format_string<T...> fmt, T&&... args)
    {
        log(logging::Level::Warning, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void log_e(fmt::format_string<T...> fmt, T&&... args)
    {
        log(logging::Level::Error, fmt, std::forward<T>(args)...);
    }

private:
    template <typename... T>
    void log(logging::Level level, fmt::format_string<T...> fmt, T&&... args)
    {
        publish_event<logging::Entry>({
            .timestamp = std::chrono::system_clock::now(),
            .level = level,
            .thread_id = this_thread().get_label(),
            .msg = fmt::format(fmt, std::forward<T>(args)...)
        });
    }

public:
    using ThreadFunc = std::function<void(entt::registry& registry)>;
    void create_thread(std::string&& label, ThreadFunc&& f, entt::registry& registry);

    auto& get_threads() const { return threads; }
    auto& get_threads() { return threads; }
    auto& this_thread() const { return threads.at(std::this_thread::get_id()); }
    auto& this_thread() { return threads.at(std::this_thread::get_id()); }
private:
    std::atomic_bool dummy;
    std::unordered_map<std::thread::id, ThreadData> threads;
    std::unordered_map<std::type_index, AnyPtr> ctx;
};