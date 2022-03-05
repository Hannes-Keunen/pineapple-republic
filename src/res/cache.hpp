#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace res
{
    template <typename T>
    class Cache
    {
    public:
        template <typename... Args>
        auto emplace(const std::string& key, Args&&... args)
        {
            cache.emplace(key, std::make_shared<T>(std::forward<Args>(args)...));
            return cache.at(key);
        }

        auto get(const std::string& key) const
        {
            return cache.at(key);
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<T>> cache;
    };

} // namespace res
