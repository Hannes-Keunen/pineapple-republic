#pragma once

#include <optional>
#include <string>
#include <vector>

namespace str
{
    auto split(const std::string& str, char delim = ' ') -> std::vector<std::string>;
    auto parse_int(const std::string& str) -> std::optional<int>;
} // namespace str
