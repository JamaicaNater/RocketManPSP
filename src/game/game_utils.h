#pragma once

#include <stdint.h>

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
 * @brief Wrapper for sceKernelGetSystemTimeLow() that takes into account the 
 * time we spend paused
 * 
 * @return unsigned int 
 */
unsigned int get_time();