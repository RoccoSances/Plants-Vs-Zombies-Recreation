#include "plants.h"

//constructor
plants::plants() {
	name = "Blank";
	health = 0;
	damage = 0;
	range = 0;
}

plants::plants(string in_name, int in_health, int in_damage, int in_range) {
	name = in_name;
	health = in_health;
	damage = in_damage;
	range = in_range;
}

//methods
string plants::show_name() {
	return name;
}

int plants::show_health() {
	return health;
}

int plants::show_damage() {
	return damage;
}

int plants::show_range() {
	return range;
}

void plants::take_damage(int dmg) {
	health -= dmg;
}


//destructor
plants::~plants() {

}
