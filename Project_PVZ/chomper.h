#pragma once
#include "plants.h"
class chomper :
    public plants
{
private:

public:
    //constructor
    chomper();
    chomper(string in_name, int in_health, int in_damage, int in_range);

    //method
    int chomp();

    //destructor
    ~chomper();
};

