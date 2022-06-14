#pragma once

#include "image.h"
#include "logger/logger.h"

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

	Image get_frame(int index);

	Animation(uint32_t _rows, uint32_t _cols, uint32_t _frame_time, 
		const char * _filename);

	~Animation();
};