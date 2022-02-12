#pragma once

#include "model/crop.hpp"
#include "model/item.hpp"

#include <entt/entt.hpp>

#include <string>
#include <unordered_map>

template <typename T>
using Registry = std::unordered_map<std::string, T>;

using CropRegistry = Registry<CropType>;
using ItemRegistry = Registry<ItemType>;

struct InitData
{
    std::vector<ItemType> items;
    std::vector<CropType> crops;
};

void init_registries(entt::registry &registry, const InitData& data);
