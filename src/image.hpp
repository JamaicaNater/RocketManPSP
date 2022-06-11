#pragma once

#include <cstring>
#include "logger/logger.h"
//#include "bmp/loadbmp.h"

struct Image
{
	unsigned int * img_matrix = NULL;
	unsigned int width = 0;
	unsigned int height = 0;

	char filename[64] = {'\0'};
	Image(const char * _filename) {
		int str_size = strlen(_filename) + 1;
		if (str_size > 64) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size);
	}

	Image(unsigned int _height, unsigned int _width, unsigned int * _img_matrix,
		const char * _filename) 
	{
		height = _height;
		width = _width;
		img_matrix = _img_matrix;

		int str_size = strlen(_filename) + 1;
		if (str_size > 64) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size);
	}

	Image() {}

	~Image() {
	}
};

struct Animation
{
	private:
	public:
	// Program updated variables
	unsigned int * img_matrices = NULL;
	unsigned int width = 0;
	unsigned int height = 0;

	// User provided variables 
	unsigned int frame_time = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	char filename[64] = {'\0'};

	Image get_frame(int index) {
		// PSP_LOGGER::assert(index < rows*cols, "get_frame(index) "
		// "requested an in bounds value of %d for size %d", index, rows * cols);

		return(Image(height, width, img_matrices + (width * height * index), "Non-Loadable"));
	}

	Animation(unsigned int _rows, unsigned int _cols, unsigned int _frame_time, 
		const char * _filename) 
	{
		frame_time = _frame_time;
		
		int str_size = strlen(_filename) + 1;
		if (str_size > 64) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Filename"
		" %s too long", _filename);
		strncpy(filename, _filename, str_size);

		rows = _rows;
		cols = _cols;
	}

	~Animation() {
	}
};