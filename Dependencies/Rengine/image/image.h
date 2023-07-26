#pragma once

#include <cstdlib>
#include <cstring>
#include <math.h>
#include <stdint.h>

#include "logger/logger.h"
#include "memory/psp_malloc.h"

struct Image {
    uint32_t * img_matrix = NULL;
    uint32_t width = 0;
    uint32_t height = 0;

    char filename[64] = {'\0'};
    explicit Image(const char * _filename);

    Image(uint32_t _height, uint32_t _width, uint32_t * _img_matrix,
        const char * _filename);

    Image();

    ~Image();

    /**
     * @brief For images with a valid passed filename used so that image can be
     * declared and loaded in one line (useful for constructors)
     *
     * @return Image
     */
    Image& load();

    Image trim_edges();

    /**
     * @brief Resize an image
     *
     * @param h New height
     * @param w New width
     * @return Image
     */
    Image resize(int h, int w);

    /**
     * @brief Resize image
     *
     * @param scale Scale of new image compared with previous
     * @return Image
     */
    Image resize(float scale);

    /**
     * @brief Vertically flip image
     *
     * @return Image
     */
    Image flip_vertical();

    /**
     * @brief Blurs an image
     *
     * @return Image
     */
    Image blur();
};
