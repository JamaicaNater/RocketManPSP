#pragma once

#include <cstring>
#include <cstdlib>
#include "Vector2d.hpp"
#include "logger/logger.h"

struct Image
{
	unsigned int * img_matrix = NULL;
	unsigned int width = 0;
	unsigned int height = 0;

	char filename[128] = {'\0'};
	Image(const char * _filename) {
		int str_size = strlen(_filename) + 1;
		if (str_size > 128) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size);
	}

	Image(unsigned int _height, unsigned int _width, unsigned int * _img_matrix, const char * _filename) {
		height = _height;
		width = _width;
		img_matrix = _img_matrix;

		int str_size = strlen(_filename) + 1;
		if (str_size > 128) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size-1);
	}

	Image() {}

	~Image() {
	}
};

struct Animation
{
	private:
		unsigned int current_frame = 0;
	public:
	// Program updated variables
	unsigned int * * img_matrices = NULL;
	unsigned int last_updated = 0;
	unsigned int width = 0;
	unsigned int height = 0;

	// User provided variables 
	unsigned int frametime = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	char filename[128] = {'\0'};

	Image get_frame(int index) {
		PSP_LOGGER::assert_or_log(index < rows*cols, "get_frame(index) "
		"requested an in bounds value of %d for size %d", index, rows * cols);

		return(Image(height, width, img_matrices[index], "."));
	}

	Image get_next_frame(int time){
		if (time > last_updated + frametime){
			current_frame++;
			current_frame%=(rows * cols);
		}
		return(Image(height, width, img_matrices[current_frame], "."));
	}

	Animation(unsigned int _rows, unsigned int _cols, unsigned int _frametime, const char * _filename) {
		frametime = _frametime;
		
		int str_size = strlen(_filename) + 1;
		if (str_size > 128) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size-1);

		rows = _rows;
		cols = _rows;
	}

	~Animation() {
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



