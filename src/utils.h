#pragma once
#include "image.hpp"

/**
 * @brief takes in a range of number from -1 to 1 and maps the number a range of numbers
 * 
 * @param num value between -1 and 1
 * @param range 
 * @return float mapped value
 */
float map(float num, int range);

/**
 * @brief Converts date from big endian to little endian and vice versa
 * 
 * @param data data in original endianess
 * @return unsigned int data in new endianess
 */
unsigned int swap_endian(unsigned int data);

int get_cam_position(int player_postion, int center);

unsigned int filter(unsigned int pixel, unsigned int filter);

void blur(Image &img);