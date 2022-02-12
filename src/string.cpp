#include "string.hpp"

#include <stdexcept>

namespace str
{
    auto split(const std::string& str, char delim) -> std::vector<std::string>
    {
        size_t pos = 0;
        std::vector<std::string> result;
        while (pos != std::string::npos)
        {
            while (std::isspace(str[pos]) > 0)
            {
                ++pos;
            }

            size_t next = str.find(' ', pos);
            std::string part = str.substr(pos, next - pos);
            result.push_back(part);
            pos = next;
        }
        return result;
    }

    auto parse_int(const std::string& str) -> std::optional<int>
    {
        try
        {
            return std::stoi(str);
        }
        catch(const std::invalid_argument& e)
        {
            return std::nullopt;
        }
        catch(const std::out_of_range& e)
        {
            return std::nullopt;
        }
        
    }

} // namespace str
