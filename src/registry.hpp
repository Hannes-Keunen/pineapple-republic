#pragma once

#include "model/crop.hpp"
#include "model/item.hpp"

#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
using Registry = std::unordered_map<std::string, T>;

using CropRegistry = Registry<CropType>;
using ItemRegistry = Registry<ItemType>;

struct InitData
{
    std::vector<ItemType> items;
    std::vector<CropType> crops;
};

class GameState;

void init_registries(GameState& state, const InitData& data);
