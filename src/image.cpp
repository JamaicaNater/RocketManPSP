#include <cstdlib>
#include <cstring>
#include <math.h>
#include "logger/logger.h"
#include "utils.h"
#include "utils/psp_malloc.h"
#include "image.h"

Image::Image(const char * _filename) {
    int str_size = strlen(_filename) + 1;
    if (str_size > 64) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Filename"
    " %s too long", _filename);
    strncpy(filename, _filename, str_size);
}

Image::Image(uint32_t _height,uint32_t _width, uint32_t * _img_matrix,
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

// Image (const Image &other){
// 	width = other.width;
// 	height = other.height;
// 	int size = other.width * other.height * sizeof(unsigned int);
// 	can_free = other.can_free;
// 	img_matrix = (unsigned int*)psp_malloc(size);
// 	memcpy(img_matrix, other.img_matrix, size);
// }

Image::Image() {}

Image::~Image() {
}

/*
    Implementation of the nearest neighbor resize algorithm
    https://towardsdatascience.com/image-processing-image-scaling-algorithms-ae29aaa6b36c
*/
void Image::resize(int h, int w){
    uint32_t * new_img = (uint32_t *)psp_malloc(h * w * sizeof(uint32_t));
    PSP_LOGGER::assert(img_matrix, "Image to resize initialized");
    PSP_LOGGER::assert(new_img, "Success in creating resize img");

    float scale_x = w/(float)width;
    float scale_y = h/(float)height;
    for (int y = 0; y < h; y++){
        for (int x = 0; x < w; x++){
            new_img[y * w + x] = img_matrix[(int)round(y/scale_y) * width + (int)round(x/scale_x)];
        }
    }
    psp_free(img_matrix);
    img_matrix = new_img;
    height = h;
    width = w;
}

void Image::resize(float scale){
    resize(scale*height, scale*width);
}

/*
The following is an implementation of the Box Blur 
(https://en.wikipedia.org/wiki/Box_blur) algorithm, in the implementation of
the algorithm we take the neighbors (3 x 3) of a matrix value and average 
their values together

This implementation blurs the matrix inplace
*/
void Image::blur(){
    const uint32_t neighborhood_size_max = 9;
    uint32_t neighborhood_size;
    uint32_t to_avg[neighborhood_size_max];

    int start_i, end_i, start_j, end_j, index;

    uint32_t prev_row[width];
    uint32_t prev_item;

    for (uint32_t y = 0; y <= height; y++){
        if (y>0) memcpy(prev_row, img_matrix + (y-1)*width, width * sizeof(uint32_t));
        for (uint32_t x = 0; x < width; x++){
            if (x>0) prev_item = img_matrix[y*width + x-1];		
            start_i = (y > 0) ? -1 : 0;
            start_j = (x > 0) ? -1 : 0;

            end_i = (y < height) ? 1 : 0;
            end_j = (x < width) ? 1 : 0;

            index = 0;
            for (int i = start_i; i <= end_i; i++){
                for (int j = start_i; j <= end_j; j++){
                    // We have already modified the top values
                    if (i == -1) to_avg[index] = prev_row[(x+j)]; 
                    // We have already modified the left value
                    else if (i == 0 && j == -1) to_avg[index] = prev_item;
                    // We have not modified these values
                    else to_avg[index] = img_matrix[(y+i) * width + (x+j)];
                    index++;
                }
            }
            neighborhood_size = (end_i - start_i + 1) * (end_j - start_j + 1);
            img_matrix[y*width + x] = average_pixels(to_avg, neighborhood_size);
        }
    } 
}