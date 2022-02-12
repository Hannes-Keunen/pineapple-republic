#pragma once

#include <string>

struct TreeType
{
    std::string id;
    std::string texid;
    std::string name_key;
    std::string produce_id;
    int grow_time;
    int harvest_interval;
};

// Tree component
struct Tree
{
    std::string tree_id;
    int time_since_harvest;
};

// Baby tree component
struct TreeSapling
{
    std::string tree_id;
    int age;
};
