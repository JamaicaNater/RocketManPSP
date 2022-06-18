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

    Menu set_pos(Position pos, int padding_x = 0, int padding_y = 0);
    
    /**
     * @brief 
     * 
     * @param pos Where on the Menu should the component go
     * @param comp Component being added
     * @param padding_x Additional x from the position
     * @param padding_y Additional y from the position
     * @return std::pair<int,int> 
     * pair.first = index of the first element of the group in component vector
     * pair.second = index of the group in group vector
     */
    std::pair<int,int> add_component(Position pos, Component comp, int padding_x = 0, 
        int padding_y = 0);
    
    /**
     * @brief Adds a list of components to the menu, grouping them together
     * 
     * @param pos Where on the Menu should the component go
     * @param arr Array of components to be added
     * @param grouping Type of grouping
     * @param spacing Space between components
     * @param padding_x Additional x from the position
     * @param padding_y Additional y from the position
     * @param rows 
     * @param cols 
     * @param row_major is the value row major
     * @return std::pair<int,int> 
     * pair.first = index of the first element of the group in component vector
     * pair.second = index of the group in group vector
     */
    std::pair<int,int> add_component_group(Position pos, std::vector<Component> arr, 
        Grouping grouping, int spacing = 1, int padding_x = 0, int padding_y = 0, 
        int rows = 0, int cols = 0, bool row_major = true);

    /**
     * @brief Draws panel type component
     * 
     * @param comp 
     */
    void draw_panel(Component comp);

    /**
     * @brief Draws image type component
     * 
     * @param comp 
     */
    void draw_img(Component comp);

    /**
     * @brief Draws text type component
     * 
     * @param comp 
     */
    void draw_text(Component comp);
    
    /**
     * @brief Updates the displayed image of the menu
     * 
     */
    void update();

    /**
     * @brief close the menu TODO: finish
     * 
     */
    void close();

    /**
     * @brief Get the selectable components in a given array
     * Primary use is the find which component are selectable of the components
     * in a groups' pointer vector
     * @param arr 
     * @return std::vector<int> indices of the selectable components
     */
    std::vector<int> get_selectable_components(std::vector<Component *> arr);

    void select_next(Direction direction);
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

    /**
     * @brief Highlights a pixel if component is selected TODO: increase effiecincy
     * 
     * @param comp 
     * @param pixel 
     * @return uint32_t highlighted value or original value
     */
    uint32_t highlight_selection(Component comp, uint32_t pixel);

    /**
     * @brief Convert relative positions to actual coordinates on a Menu or 
     * screen
     * 
     * @param pos Relative position
     * @param height_obj Height of the object we are placing
     * @param width_obj Width of the object we are placing
     * @param height_pan Height of the panel we are placing object on
     * @param width_pan Width of the panel we are placing object on
     * @param screen_coord whether or not the are placing the object on the screen
     * @param padding_x additional x
     * @param padding_y additional y if it is a screen coord additional y places 
     * it lower
     * @return Vector2d (x,y) coords
     */
    Vector2d pos_to_coord(Position pos, unsigned int height_obj, 
        unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
        bool screen_coord, int padding_x = 0, int padding_y = 0 );
    
    /**
     * @brief Adds a grid filling up each columns then proceeding to the next 
     * column
     * 
     * @param pos Relative postion
     * @param arr Array of Components
     * @param spacing Space between components
     * @param padding_x Additional x
     * @param padding_y Additional y
     * @param rows 
     * @param cols 
     */
    void add_grid_col_major(Position pos, std::vector<Component> arr,
        int spacing = 1, int padding_x = 0, int padding_y = 0,
        int rows = 0, int cols = 0);

    /**
     * @brief Adds a grid filling up each columns then proceeding to the next 
     * column
     * 
     * @param pos Relative postion
     * @param arr Array of Components
     * @param spacing Space between components
     * @param padding_x Additional x
     * @param padding_y Additional y
     * @param rows 
     * @param cols 
     */
    void add_grid_row_major(Position pos, std::vector<Component> arr, 
        int spacing = 1, int padding_x = 0, int padding_y = 0,
        int rows = 0, int cols = 0);
};
