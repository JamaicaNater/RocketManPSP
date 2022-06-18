#pragma once

#include <cstdlib>
#include <cstring>
#include <math.h>
#include <stdint.h>

#include "logger/logger.h"
#include "memory/psp_malloc.h"

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

	Image resize(int h, int w);
	Image resize(float scale);
	Image blur();
};