#pragma once

#include <stdint.h>

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
