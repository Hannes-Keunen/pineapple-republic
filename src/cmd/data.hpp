#pragma once

#include <string>

enum class PlantType
{
    Crop,
    Tree
};

struct CmdPlantData
{
    PlantType type;
    std::string plant_id;
    int x;
    int y;
};

struct CmdHarvestData
{
    int x;
    int y;
};
