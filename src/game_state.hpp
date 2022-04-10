#pragma once

#include "any.hpp"
#include "event.hpp"

#include <entt/entt.hpp>

#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <utility>

using Ecs = entt::registry;

class GameState
{
public: // state variables
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

public: // events
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
    auto& get_ecs() { return ecs; }

    // TODO: try to synchronize parts of graphics/simulation instead of exclusive lock
    [[nodiscard]] auto begin_frame() { return std::lock_guard<std::mutex>(frame_mutex); }
private:
    Ecs ecs;
    std::unordered_map<std::type_index, AnyPtr> ctx;
    std::mutex frame_mutex;
};