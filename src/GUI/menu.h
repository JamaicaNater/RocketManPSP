#pragma once

#include "component.h"

#include <cstdlib>
#include <vector>

#include "utils/psp_malloc.h"

#include "../image/image.h"
#include "../graphics/gfx.hpp"
#include "../utils.h"
#include "../controls/control_reader.h"
#include "../logger/logger.h"

class Menu
{
public:   
    enum Grouping{GRID, VERTICAL_LIST, HORIZONTAL_LIST};
    enum Direction{UP, DOWN, LEFT, RIGHT};
    
    unsigned int x, y, height, width;
    uint32_t background_color;
    uint32_t selected_color = 0x0000AA;

    ControlReader control_reader; // TOdo init

    std::function<void(Menu &self)> on_open;

    Image gui;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
        unsigned int _width, uint32_t color);
    Menu(Position _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x = 0, int padding_y = 0);

    ~Menu();

    std::pair<int,int> add_component(Position pos, Component comp, int padding_x = 0, 
        int padding_y = 0);
 
    std::pair<int,int> add_component_group(Position pos, std::vector<Component> arr, 
        Grouping grouping, int spacing = 1, int padding_x = 0, int padding_y = 0, 
        int rows = 0, int cols = 0, bool row_major = false);

    void draw_panel(Component comp);

    void draw_img(Component comp);

    void draw_text(Component comp);

    void set_pos(Position pos, int padding_x = 0, int padding_y = 0);

    void update();

    void close();

    std::vector<int> get_selectable_components();
    std::vector<int> get_selectable_components(std::vector<Component *> arr);

    void select_component(int comp_id);

    void select_next();
    void select_prev();
private:
// TODO ID vs index
    int next_id = 0;
    int next_group_id = 0;

    int selected_comp_id = 0;
    int selected_group_id = 0;

    std::vector<Component> components;
    struct GroupInfo
    {
        int num_components;
        bool row_major;
        int rows;
        int cols;
        std::vector<Component*> components;

        GroupInfo(bool _row_major, int _rows, int _cols, 
            std::vector<Component*> _components
        ) {
            row_major = _row_major;
            rows = _rows;
            cols = _cols;
            components = _components;
        }
    };

    std::vector<GroupInfo> groups;

    uint32_t highlight_selection(Component comp, uint32_t pixel);
    Vector2d pivot_to_coord(Position pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );
    void add_grid_col_major(Position pos, std::vector<Component> arr,
        int spacing = 1, int padding_x = 0, int padding_y = 0,
        int rows = 0, int cols = 0);
    void add_grid_row_major(Position pos, std::vector<Component> arr, 
        int spacing = 1, int padding_x = 0, int padding_y = 0,
        int rows = 0, int cols = 0);
};
