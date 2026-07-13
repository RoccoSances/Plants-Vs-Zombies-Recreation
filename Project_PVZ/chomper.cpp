#include "chomper.h"
//constructor
chomper::chomper() : plants("chomper", 100, 50, 1) {


}
chomper::chomper(string in_name, int in_health, int in_damage, int in_range)
	: plants(in_name, in_health, in_damage, in_range) {

}

//method
int chomper::chomp() {
	return damage;
}

//destructor
chomper::~chomper() {

}
