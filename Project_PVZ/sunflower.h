#pragma once
#include "plants.h"
class sunflower :
    public plants
{
private:    
    //attribute
    int sun_gain;   //skill, increases sun every round

public:
    //constructor
    sunflower();
    sunflower(string in_name, int in_health, int in_damage, int in_range, int in_sun_gain);

    //method
    int produce_sun();

    //destructor
    ~sunflower();
};

