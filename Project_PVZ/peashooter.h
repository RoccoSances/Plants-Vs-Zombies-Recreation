#pragma once
#include "plants.h"
class peashooter :
    public plants
{
private:


public:
    //constructor
    peashooter();
    peashooter(string in_name, int in_health, int in_damage, int in_range);

    //method
    int attack();

    //destructor
    ~peashooter();
};

