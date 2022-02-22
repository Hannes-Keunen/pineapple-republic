#pragma once

#include <game_state.hpp>

#include <fmt/format.h>

#include <chrono>
#include <string>
#include <string_view>

namespace logging
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
        std::string thread_id;
        std::string msg;
    };

} // namespace log

template<>
struct fmt::formatter<logging::Level> : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(logging::Level api, FormatContext& ctx) {
        switch (api) {
        case logging::Level::Trace: return formatter<std::string_view>::format("T", ctx);
        case logging::Level::Debug: return formatter<std::string_view>::format("D", ctx);
        case logging::Level::Info: return formatter<std::string_view>::format("I", ctx);
        case logging::Level::Warning: return formatter<std::string_view>::format("W", ctx);
        case logging::Level::Error: return formatter<std::string_view>::format("E", ctx);
        default: return formatter<std::string_view>::format("?", ctx);
        }
    }
};

