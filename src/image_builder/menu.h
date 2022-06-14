#pragma once

#include <cstdlib>
#include "../image/image.h"
#include "../logger/logger.h"
#include "../graphics/gfx.hpp"
#include "../utils.h"
#include "utils/psp_malloc.h"


struct Menu
{
public:   
    unsigned int x, y, height, width;
    char * text_field;
    //Component * children[4];

    unsigned int * img_matrix;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height, unsigned int _width, unsigned int color);
    ~Menu();

    Vector2d pivot_to_coord(pivots pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );

    void add_panel(pivots _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x = 0, int padding_y = 0);

    void add_img(unsigned int _x, unsigned int _y, Image _img);

    void set_pos(pivots pos, int padding_x = 0, int padding_y = 0);

    Image get_image();
};
