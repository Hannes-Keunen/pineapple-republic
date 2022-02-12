#pragma once

#include "cmd.hpp"

#include <fmt/core.h>

#include <string>

namespace cmd
{
    auto check_argc(int argc, int required) -> bool;
    auto parse_int_argument(const std::string& str, int& out) -> bool;
    
    template <typename... T>
    auto result(fmt::format_string<T...> fmt, T&&... args) -> CommandResult
    {
        return {
            .msg = fmt::format(fmt, std::forward<T>(args)...)
        };
    }
} // namespace cmd::args
