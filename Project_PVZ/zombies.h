#pragma once
#include<iostream>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;

class zombies
{
	//attribute
	string name;
	int health;
	int damage;
	int position;	//For the zombies spot on grid

public:
	//constructor
	zombies();	//default
	zombies(string in_name, int in_health, int in_damage, int in_position);

	//methods
	virtual string show_name();
	int show_health();
	int show_damage();
	int show_position();

	void move_left();
	void take_damage(int dmg);
	bool is_alive();


	//destructor
	~zombies();


};

