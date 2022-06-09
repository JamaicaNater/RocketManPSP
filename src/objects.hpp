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
	int health = 100, max_health = 100;

	Vector2d vector;
	Object() {}
	Object(Image _img) {
		image = _img;
	}

	bool off_screen() {
		return get_draw_x() > SCREEN_WIDTH_RES + 50 || get_draw_x() < -50
		|| get_draw_y() > SCREEN_WIDTH + 50 || get_draw_y() < -50;
	}

	int get_draw_x(){
		// Where to draw the player on the screen
		if (type == PLAYER && MAP_SIZE - vector.x > SCREEN_WIDTH/2){ 
			if (vector.x > SCREEN_WIDTH/2) return SCREEN_WIDTH/2 - image.width/2;
		}
		
		return vector.x - camera_x - image.width/2;
	}

	int get_draw_y(){
		return vector.y - image.height;
	}
};




