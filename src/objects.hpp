#pragma once

#include <cstring>
#include <cstdlib>

#include "Vector2d.hpp"
#include "logger/logger.h"
#include "image.hpp"
#include "globals.h"

class Object
{
	public:
	enum ObjectTypes{PLAYER = 0, ENEMY, MISSILE, EXPLOSION, NONE};

	ObjectTypes type = NONE;
	Image image;
	int draw_pos_x; // postion to draw at

	Vector2d vector;
	Object() {}
	Object(Image _img) {
		image = _img;
	}

	bool off_screen() {
		return draw_pos_x > 480 + 50 || draw_pos_x < -50
		|| vector.y > 512 + 50 || vector.y < -50;
	}

	int get_draw_x(){
		// Where to draw the player on the screen
		if (type == PLAYER && MAP_SIZE - vector.x > 512/2){ 
			if (vector.x > 512/2) return 512/2 - image.width/2;
		}
		
		return vector.x - camera_x - image.width/2;
	}

	int get_draw_y(){
		return vector.y - image.height;
	}
};

class Person: public Object 
{
	public:
	Object weapon = Object(Image("assets/player_rocket.bmp"));
	
	unsigned int jump_time;
	int jump_height = 0;
	int starting_jump_height;

	int weapon_last_fired = 0;
	int time_between_fires = 100*1000;

	Person(Image _img) {
		image = _img;
	}

	unsigned short jump_height_at(float time) {
		time *=2;
		return starting_jump_height + vector.vel_y*(time) 
		       + .5 * (Vector2d::grav * (time) * (time) );		
	}

} ;



