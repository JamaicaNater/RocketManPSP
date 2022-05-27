#pragma once

#include <cstring>
#include <cstdlib>
#include "Vector2d.hpp"

struct Image
{
	unsigned int * img_matrix = NULL;
	unsigned int width = 0;
	unsigned int height = 0;

	char * filename = NULL;
	Image(const char * _filename) {
		int str_size = strlen(_filename) + 1;
		filename = (char * )malloc(str_size);
		strncpy(filename, _filename, str_size-1);
	}

	Image() {}
};

struct Animation
{
	// Program updated variables
	unsigned int * * img_matrices = NULL;
	unsigned int last_updated = 0;
	unsigned int width = 0;
	unsigned int height = 0;

	// User provided variables 
	unsigned int frametime = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	char * filename = NULL;

	Animation(unsigned int _rows, unsigned int _cols, unsigned int _frametime, const char * _filename) {
		frametime = _frametime;
		
		int str_size = strlen(_filename) + 1;
		filename = (char * )malloc(str_size);
		strncpy(filename, _filename, str_size-1);

		rows = _rows;
		cols = _rows;
	}
};


class Object
{
	public:
	float grav = 110.00f;
	Image image;
	int draw_pos_x; // postion to draw at

	Vector2d vector;
	Object() {}
	Object(Image _img) {
		image = _img;
	}
};

class Person: public Object 
{
	public:
	Object weapon = Object(Image("assets/player_rocket.bmp"));
	
	bool jumping = false;
	unsigned int jump_time;
	int jump_height = 0;
	int starting_jump_height;

	Person(Image _img) {
		image = _img;
	}

	unsigned short jump_height_at(int microseconds) {
		

		return 1 ;
	}

} ;



