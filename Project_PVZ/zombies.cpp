#include "zombies.h"
//constructor
zombies::zombies() {
	name = "Zombie";
	health = 100;
	damage = 25;
	position = 9; // 0-9 grid
}
zombies::zombies(string in_name, int in_health, int in_damage, int in_position) {
	name = in_name;
	health = in_health;
	damage = in_damage;
	position = in_position;
}

//methods
string zombies::show_name() {
	return name;
}

int zombies::show_health() {
	return health;
}

int zombies::show_damage() {
	return damage;
}

int zombies::show_position() {
	return position;
}

void zombies::move_left() {
	if (position > 0) {
		position--;
	}
}

void zombies::take_damage(int dmg) {
	health -= dmg;
	if (health < 0) {
		health = 0;
	}

}

bool zombies::is_alive() {
	return(health > 0);
}


//destructor
zombies::~zombies() {

}