#pragma once

#include <string>

struct AnimalType
{
    std::string id;
    std::string texid;
    std::string name_key;
    std::string produce_id;
    int grow_time;
    int harvest_interval;
};

// Animal component
struct Animal
{
    std::string animal_id;
    int time_since_harvest;
};

// Baby animal component
struct BabyAnimal
{
    std::string animal_id;
    int age;
};
