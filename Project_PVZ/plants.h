#pragma once
#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;

class plants
{
protected:
	//attribute
	string name;
	int health;
	int damage;
	int range;

public:
	//constructor
	plants();	//default
	plants(string in_name, int in_health, int in_damage, int in_range);

	//methods
	virtual string show_name();
	int show_health();
	int show_damage();
	int show_range();

	void take_damage(int dmg);

	//destructor
	virtual ~plants();

};

