#pragma once

#include <cstdlib>

#include "component.h"

#include "../image/image.h"
#include "../logger/logger.h"
#include "../graphics/gfx.hpp"
#include "../utils.h"
#include "utils/psp_malloc.h"


class Menu
{
public:   
    unsigned int x, y, height, width;

    Component * children[4];
    unsigned int * img_matrix;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
        unsigned int _width, uint32_t color);
    Menu(pivots _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x = 0, int padding_y = 0);

    ~Menu();

    Vector2d pivot_to_coord(pivots pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );

    void add_component(pivots pos, Component comp, int padding_x = 0, int padding_y = 0);

    void add_panel(unsigned int _x, unsigned int _y, Component comp);

    void add_img(unsigned int _x, unsigned int _y, Image _img);

    void set_pos(pivots pos, int padding_x = 0, int padding_y = 0);

    Image get_image();
};
