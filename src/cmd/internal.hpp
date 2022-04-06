#pragma once

#include "cmd.hpp"

#include <fmt/core.h>

#include <string>

namespace cmd
{
    auto check_argc(int argc, int required) -> bool;
    auto parse_int_argument(const std::string& str, int& out) -> bool;
    
    template <typename... T>
    auto success_result(fmt::format_string<T...> fmt, T&&... args) -> CommandResult
    {
        return {
            .state = CommandResultState::Ok,
            .msg = fmt::format(fmt, std::forward<T>(args)...)
        };
    }
    
    template <typename... T>
    auto error_result(fmt::format_string<T...> fmt, T&&... args) -> CommandResult
    {
        return {
            .state = CommandResultState::Error,
            .msg = fmt::format(fmt, std::forward<T>(args)...)
        };
    }
} // namespace cmd::args
