#include "sunflower.h"
//constructor
sunflower::sunflower() : plants("Sunflower", 50, 0, 0) {
	sun_gain = 25;
}

sunflower::sunflower(string in_name, int in_health, int in_damage, int in_range, int in_sun_gain) 
	: plants(in_name, in_health, 0, in_range) {
	sun_gain = in_sun_gain;
}

//method
int sunflower::produce_sun() {
	return sun_gain;
}

//destructor
sunflower::~sunflower(){

}
