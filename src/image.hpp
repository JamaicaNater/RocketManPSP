#pragma once
#include <cstdlib>
#include <cstring>
#include <math.h>
#include "logger/logger.h"
#include <stdint.h>
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
		//free(img_matrix);
	}

	/*
		Implementation of the nearest neighbor resize algorithm
		https://towardsdatascience.com/image-processing-image-scaling-algorithms-ae29aaa6b36c
	*/
	void resize(int h, int w){
		uint32_t * new_img = (uint32_t *)malloc(h * w * sizeof(uint32_t));
		PSP_LOGGER::assert(img_matrix, "Image to resize initialized");
		PSP_LOGGER::assert(new_img, "Success in creating resize img");

		float scale_x = w/(float)width;
		float scale_y = h/(float)height;
		for (int y = 0; y < h; y++){
			for (int x = 0; x < w; x++){
				new_img[y * w + x] = img_matrix[(int)round(y/scale_y) * width + (int)round(x/scale_x)];
			}
		}
		free(img_matrix);
		img_matrix = new_img;
		height = h;
		width = w;
	}

	void resize(float scale){
		resize(scale*height, scale*width);
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

		return(Image(height, width, img_matrices + (width * height * index), 
			"Non-Loadable Animation frame"));
	}

	Animation(unsigned int _rows, unsigned int _cols, unsigned int _frame_time, 
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