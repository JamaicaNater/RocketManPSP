#pragma once

#include <cstring>
#include <cstdlib>

#include "Vector2d.hpp"
#include "logger/logger.h"
#include "../image/image.h"
#include "globals.h"

class Object
{
	public:
	enum ObjectTypes{PLAYER = 0, ENEMY, MISSILE, EXPLOSION, WEAPON, NONE};

	ObjectTypes type = NONE;
	Image image;
	int draw_pos_x; // postion to draw at
	int health = 100, max_health = 100;

	unsigned int next_damage_event = 0;
	unsigned int invincibility_time = 10 * MILLISECOND;

	Vector2d vector;
	
	Object();
	
	Object(Image _img);

	void damage_object(int damage);

	bool off_screen();

	int get_draw_x();

	int get_draw_y();
};




