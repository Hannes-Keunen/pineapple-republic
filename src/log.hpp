#pragma once

#include <game_state.hpp>

#include <fmt/format.h>

#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <thread>

namespace logger
{
    enum class Level
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error
    };

    using TimeStamp = std::chrono::system_clock::time_point;

    struct Entry
    {
        TimeStamp timestamp;
        Level level;
        std::thread::id thread_id;
        std::string msg;
    };

    void publish(const Entry& entry);
    void subscribe(std::function<void(const Entry&)> callback);
    void drain();

    template <typename... T>
    void print(logger::Level level, fmt::format_string<T...> fmt, T&&... args)
    {
        publish({
            .timestamp = std::chrono::system_clock::now(),
            .level = level,
            .thread_id = std::this_thread::get_id(),
            .msg = fmt::format(fmt, std::forward<T>(args)...)
        });
    }

    template <typename... T>
    void t(fmt::format_string<T...> fmt, T&&... args)
    {
        print(logger::Level::Trace, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void d(fmt::format_string<T...> fmt, T&&... args)
    {
        print(logger::Level::Debug, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void i(fmt::format_string<T...> fmt, T&&... args)
    {
        print(logger::Level::Info, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void w(fmt::format_string<T...> fmt, T&&... args)
    {
        print(logger::Level::Warning, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void e(fmt::format_string<T...> fmt, T&&... args)
    {
        print(logger::Level::Error, fmt, std::forward<T>(args)...);
    }

} // namespace log

template<>
struct fmt::formatter<logger::Level> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(logger::Level api, FormatContext& ctx) {
        switch (api) {
        case logger::Level::Trace: return formatter<std::string_view>::format("Trace", ctx);
        case logger::Level::Debug: return formatter<std::string_view>::format("Debug", ctx);
        case logger::Level::Info: return formatter<std::string_view>::format("Info", ctx);
        case logger::Level::Warning: return formatter<std::string_view>::format("Warning", ctx);
        case logger::Level::Error: return formatter<std::string_view>::format("Error", ctx);
        default: return formatter<std::string_view>::format("???", ctx);
        }
    }
};

