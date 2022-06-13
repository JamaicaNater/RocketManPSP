#pragma once
#include "image.hpp"
#include <pspctrl.h>

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

unsigned int tint_pixel(unsigned int pixel, unsigned int tint);

unsigned int average_pixels( unsigned int * arr, unsigned int size);

void wait_button_release(SceCtrlData &ctrlData, PspCtrlButtons button);

unsigned int get_time();