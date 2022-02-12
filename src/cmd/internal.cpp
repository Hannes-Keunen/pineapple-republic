#include "internal.hpp"

#include "string.hpp"

#include <iostream>

namespace cmd
{
    auto check_argc(int argc, int required) -> bool
    {
        if (argc != required)
        {
            std::cout << "invalid number of arguments: required " << required << ", found " << argc << std::endl;
            return false;
        }
        return true;
    }

    auto parse_int_argument(const std::string& str, int& out) -> bool
    {
        auto res = str::parse_int(str);
        if (res == std::nullopt)
        {
            return false;
        }
        else
        {
            out = res.value();
            return true;
        }
    }

} // namespace cmd::args