#pragma once

#include <cstdlib>
#include <cstring>
#include <math.h>
#include "logger/logger.h"
#include "utils.h"
#include "utils/psp_malloc.h"
#include <stdint.h>
//#include "bmp/loadbmp.h"

struct Image
{
	uint32_t * img_matrix = NULL;
	uint32_t width = 0;
	uint32_t height = 0;

	char filename[64] = {'\0'};
	Image(const char * _filename);

	Image(uint32_t _height, uint32_t _width, uint32_t * _img_matrix,
		const char * _filename);

	Image();

	~Image();

	void resize(int h, int w);
	void resize(float scale);
	void blur();
};

struct Animation
{
	private:
	public:
	// Program updated variables
	uint32_t * img_matrices = NULL;
	uint32_t width = 0;
	uint32_t height = 0;

	// User provided variables 
	uint32_t frame_time = 0;
	uint32_t rows = 0;
	uint32_t cols = 0;
	char filename[64] = {'\0'};

	Image get_frame(int index) {
		// PSP_LOGGER::assert(index < rows*cols, "get_frame(index) "
		// "requested an in bounds value of %d for size %d", index, rows * cols);

		return(Image(height, width, img_matrices + (width * height * index), 
			"Non-Loadable Animation frame"));
	}

	Animation(uint32_t _rows, uint32_t _cols, uint32_t _frame_time, 
		const char * _filename
	){
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