#pragma once

#include "component.h"

#include <cstdlib>
#include <vector>

#include "utils/psp_malloc.h"

#include "../image/image.h"
#include "../logger/logger.h"
#include "../graphics/gfx.hpp"
#include "../utils.h"
#include "../controls/control_reader.h"

class Menu
{
public:   
    unsigned int x, y, height, width;
    uint32_t background_color;

    std::vector<Component> components;

    ControlReader control_reader; // TOdo init

    std::function<void(Menu &self)> on_open;

    Image gui;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
        unsigned int _width, uint32_t color);
    Menu(pivots _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x = 0, int padding_y = 0);

    ~Menu();

    void add_component(pivots pos, Component comp, int padding_x = 0, int padding_y = 0);

    void draw_panel(Component comp);

    void draw_img(Component comp);

    void set_pos(pivots pos, int padding_x = 0, int padding_y = 0);

    void update();

    void close();
private:
    Vector2d pivot_to_coord(pivots pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );
};
