#include "image/image_utils.h"

unsigned int average(unsigned int * arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

uint32_t tint_pixel(uint32_t pixel, uint32_t tint) {
    uint8_t red_p = (pixel&0x000000FF), red_f = (tint&0x000000FF),
        green_p = (pixel&0x0000FF00) >> 8, green_f = (tint&0x0000FF00) >> 8,
        blue_p = (pixel&0x00FF0000) >> 16, blue_f = (tint&0x00FF0000) >> 16,
        red_r, blue_r, green_r;

    red_r = (red_p + (uint32_t)red_f < UINT8_MAX) ? red_p + red_f : UINT8_MAX;
    green_r = (red_p + (uint32_t)green_f < UINT8_MAX) ? green_p + green_f : UINT8_MAX;
    blue_r = (red_p + (uint32_t)blue_f < UINT8_MAX) ? blue_p + blue_f : UINT8_MAX;

    return red_r | green_r << 8 | blue_r << 16;
}

uint32_t average_pixels(uint32_t *arr, uint32_t size) {
    uint32_t red_arr[size], blue_arr[size], green_arr[size];
    uint8_t red, green, blue;

    for (uint32_t i = 0; i < size; i++) {
        red_arr[i] = (arr[i]&0x000000FF);
        green_arr[i] = (arr[i]&0x0000FF00) >> 8;
        blue_arr[i] = (arr[i]&0x00FF0000) >> 16;
    }

    red = average(red_arr, size);
    blue = average(blue_arr, size);
    green = average(green_arr, size);

    return red | green << 8 | blue << 16;
}
