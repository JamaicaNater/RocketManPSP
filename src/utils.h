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

/**
 * @brief Get the cam position given the player position. We generally want the 
 * player to always be locatated at the center of the screen however, when the 
 * player is at the edge of the map we must make use of formulas to determine 
 * the cameras location. 
 * 
 * @param player_postion 
 * @param center 
 * @return int 
 */
int get_cam_position(int player_postion, int center);

/**
 * @brief Wait for a button to be released if it is currently pressed
 * 
 * @param ctrlData - interface to the PSP control input
 * @param button - button we wait for, these can be OR'ed together for mulitple
 */
void wait_button_release(SceCtrlData &ctrlData, PspCtrlButtons button);

/**
 * @brief Wrapper for sceKernelGetSystemTimeLow() that takes into account the 
 * time we spend paused
 * 
 * @return unsigned int 
 */
unsigned int get_time();

/**
 * @brief TODO: find out what i want this to be
 * 
 * @param pixel 
 * @param tint 
 * @return uint32_t 
 */
uint32_t tint_pixel(uint32_t pixel, uint32_t tint);

/**
 * @brief Takes in an array of pixel values and averages them together
 * 
 * @param arr 
 * @param size 
 * @return uint32_t - average value
 */
uint32_t average_pixels(uint32_t * arr, uint32_t size);

/**
 * @brief Convert pixel from ARGB (pc) to ABGR (psp) likely has to do with 
 * endianness differences
 * 
 * @param data 
 * @return uint32_t 
 */
uint32_t format_pixel(uint32_t data);