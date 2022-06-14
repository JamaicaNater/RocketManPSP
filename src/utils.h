#pragma once
#include "image/image.h"
#include <pspctrl.h>
#include "stdint.h"

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

void wait_button_release(SceCtrlData &ctrlData, PspCtrlButtons button);

unsigned int get_time();

uint32_t tint_pixel(uint32_t pixel, uint32_t tint);

uint32_t average_pixels(uint32_t * arr, uint32_t size);

uint32_t format_pixel(uint32_t data);