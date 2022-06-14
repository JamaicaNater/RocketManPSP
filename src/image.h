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