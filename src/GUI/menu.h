#pragma once

#include "component.h"

#include <utility>
#include <cstdlib>
#include <vector>

#include "image/image.h"
#include "controls/control_reader.h"
#include "physics/vector2d.h"

class Menu {
 public:
    enum Grouping{GRID, VERTICAL_LIST, HORIZONTAL_LIST};
    enum Direction{UP, DOWN, LEFT, RIGHT};

    enum CursorPosition {
        FIRST_COL     = 0x000001,
        LAST_COL      = 0x000008,
        FIRST_ROW     = 0x000010,
        LAST_ROW      = 0x000020,
    };
    uint8_t cursor_position;

    const static int MAX_COMPONENTS = 64;

    unsigned int x, y, height, width;
    uint32_t background_color;
    uint32_t selected_color = 0x0000AA;
    char menu_name[32] = "";

    ControlReader control_reader;  // Todo init

    std::function<void(Menu &self)> on_open;

    Image gui;

    Menu(unsigned int _x, unsigned int _y, unsigned int _height,
        unsigned int _width, uint32_t color);
    Menu(Position _pos, unsigned int _height, unsigned int _width,
        uint32_t _color, int padding_x = 0, int padding_y = 0);
    ~Menu();

    /**
     * @brief Set the name of the menu for debugging
     *
     * @param name
     * @return Menu
     */
    Menu set_name(const char * _name);

    /**
     * @brief Setup basic control for navigating menus up, down, left, right
     * and cross
     *
     */
    void setup_basic_controls();

    /**
     * @brief Set the position of the menu
     *
     * @param pos Position to move to
     * @param padding_x additional x
     * @param padding_y additional y
     * @return Menu self
     */
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
    std::pair<int, int> add_component(Position pos, Component comp,
        int padding_x = 0, int padding_y = 0);

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
    std::pair<int, int> add_component_group(Position pos,
        std::vector<Component> arr, Grouping grouping, int spacing = 1,
        int padding_x = 0, int padding_y = 0, int rows = 0, int cols = 0,
        bool row_major = true);

    /**
     * @brief Updates the displayed image of the menu
     *
     */
    void update();

    /**
     * @brief Draws the menu to screen and swaps buffers TODO: menu handler
     *
     */
    void draw_and_swap_buffers();

    /**
     * @brief close the menu TODO: finish
     *
     */
    void close();

    /**
     * @brief Set the cursor position object
     * used to determine if the current selection is in the top or bottom row
     * or in the first or last column
     */
    void set_cursor_position();

    /**
     * @brief Set the group that is being selected from
     *
     * @param group_index
     */
    void set_selection_group(int group_index);

    /**
     * @brief Move to the next group (wrap around if out of bounds)
     *
     */
    void next_group();

    /**
     * @brief Move to previous group (wrap around if out of bounds)
     *
     */
    void prev_group();

    /**
     * @brief Selects the next component in a given direction
     *
     * @param direction up down left right
     */
    void select_next(Direction direction);

    /**
     * @brief Activate the on click function for currently selected item
     *
     */
    void click_selection();

    /**
     * @brief Get component at an index
     *
     * @param index
     * @return Component*
     */
    Component * get_component(int index);

    /**
     * @brief Get the selected component
     *
     * @return Component*
     */
    Component * get_selected_component();

 private:
    int selected_comp = 0;
    int selected_group = 0;

    std::vector<Component> components;
    struct GroupInfo {
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
     * @brief Get the selectable components in a given array
     * Primary use is the find which component are selectable of the components
     * in a groups' pointer vector
     * @param arr
     * @return std::vector<int> indices of the selectable components
     */
    std::vector<int> get_selectable_components(std::vector<Component *> arr);

    /**
     * @brief count number of selectable components in a group
     *
     * @param group group number
     * @return int number of selectable components
     */
    int count_selectable_components(int group);

    /**
     * @brief Perform the select / deselect method on the component that is
     * currently selected
     *
     * @param pol 0 - deselect 1 - select
     * @return int 1 on success -1 on failure
     */
    int set_selection_polarity(int pol);

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
        bool screen_coord, int padding_x = 0, int padding_y = 0);

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
