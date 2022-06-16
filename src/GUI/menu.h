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
    enum Grouping{GRID, VERTICAL_LIST, HORIZONTAL_LIST};
    unsigned int x, y, height, width;
    uint32_t background_color;

    std::vector<Component> components;

    ControlReader control_reader; // TOdo init

    std::function<void(Menu &self)> on_open;

    Image gui;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
        unsigned int _width, uint32_t color);
    Menu(Position _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x = 0, int padding_y = 0);

    ~Menu();

    void add_component(Position pos, Component comp, int padding_x = 0, 
        int padding_y = 0);
 
    void add_component_group(Position pos, std::vector<Component> arr, 
        Grouping grouping, int spacing = 1, int padding_x = 0, int padding_y = 0, 
        int rows = 0, int cols = 0);

    void draw_panel(Component comp);

    void draw_img(Component comp);

    void set_pos(Position pos, int padding_x = 0, int padding_y = 0);

    void update();

    void close();
private:
    Vector2d pivot_to_coord(Position pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );

    void add_vertical_list(Position pos, std::vector<Component> arr, int spacing = 1, int padding_x = 0, int padding_y = 0);
    void add_horizontal_list(Position pos, std::vector<Component> arr, int spacing = 1, int padding_x = 0, int padding_y = 0);
    void add_grid(Position pos, std::vector<Component> arr, Grouping grouping, 
        int spacing = 1, int padding_x = 0, int padding_y = 0, int rows = 0, 
        int cols = 0);
};
