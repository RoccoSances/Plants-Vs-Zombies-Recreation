#include "peashooter.h"
//constructor
peashooter::peashooter() : plants("peashooter", 75, 25, 3) {


}
peashooter::peashooter(string in_name, int in_health, int in_damage, int in_range) 
	: plants(in_name, in_health, in_damage, in_range) {

}

//method
int peashooter::attack() {
	return damage;
}

//destructor
peashooter::~peashooter() {

}
