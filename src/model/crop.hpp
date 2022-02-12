#pragma once

#include <string>

struct CropType
{
    std::string id;
    std::string texid;
    std::string name_key;
    std::string produce_id;
    uint64_t grow_time;
};

// Crop component
struct Crop
{
    std::string type;
    uint64_t age;
};
