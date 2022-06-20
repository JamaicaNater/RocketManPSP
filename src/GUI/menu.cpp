#include "menu.h"

#include <math.h>

#include "globals.h"
#include "graphics/gfx.hpp"
#include "text_builder.h"

Menu::Menu(unsigned int _x, unsigned int _y, unsigned int _height,
    unsigned int _width, uint32_t _color)
        : gui(_height, _width,
            (uint32_t *)psp_malloc(_width * _height * sizeof(uint32_t)), "menu"
) {
    components.reserve(MAX_COMPONENTS);
    x = _x;
    y = _y;
    height = _height;
    width = _width;
    background_color = _color;
}

Menu::Menu(Position _pos, unsigned int _height, unsigned int _width,
    uint32_t _color, int padding_x/* = 0*/, int padding_y/* = 0*/)
        : gui(_height, _width,
            (uint32_t *)psp_malloc(_width * _height * sizeof(uint32_t)), "menu"
) {
    Vector2d vec = pos_to_coord(_pos, _height, _width, SCREEN_HEIGHT,
        SCREEN_WIDTH_RES, true, padding_x, padding_y);
    components.reserve(MAX_COMPONENTS);
    x = vec.x;
    y = vec.y;
    height = _height;
    width = _width;
    background_color = _color;
}

Menu::~Menu() {
//TODO DFS style dealloc
}

Menu Menu::set_name(const char * _name) {
    strncpy(menu_name, _name, 31);

    return *this;
}

void Menu::setup_basic_controls(){
    control_reader.on_button_press_up = [this]() {
        select_next(Menu::UP);
        control_reader.wait_button_release(PSP_CTRL_UP);
    };

    control_reader.on_button_press_down = [this]() {
        select_next(Menu::DOWN);
        control_reader.wait_button_release(PSP_CTRL_DOWN);
    };

    control_reader.on_button_press_left = [this]() {
        select_next(Menu::LEFT);
        control_reader.wait_button_release(PSP_CTRL_LEFT);
    };

    control_reader.on_button_press_right = [this]() {
        select_next(Menu::RIGHT);
        control_reader.wait_button_release(PSP_CTRL_RIGHT);
    };

    control_reader.on_button_press_cross = [this]() {
        click_selection();
        control_reader.wait_button_release(PSP_CTRL_CROSS);
    };
}

inline uint32_t Menu::highlight_selection(Component comp, uint32_t pixel) {
    return pixel + comp.selected * selected_color;
}

Vector2d Menu::pos_to_coord(Position pos, unsigned int height_obj,
    unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
    bool screen_coord, int padding_x/* = 0*/, int padding_y/* = 0*/
) {
    // Center of the panel we are placing the object on
    int pan_cen_x = width_pan/2;
    int pan_cen_y = height_pan/2;

    int _x,_y;

    // screen_coord: if the object we are getting the coords for are the screen
    // we have to recognize that a y value of 0 refers to the top of the screen
    switch (pos) {
    case CENTER:
        _x = pan_cen_x - width_obj/2 + padding_x;
        _y = pan_cen_y - height_obj/2 + padding_y;
        break;
    case CENTER_LEFT:
        _x = 0 + padding_x;
        _y = pan_cen_y - height_obj/2 + padding_y;
        break;
    case CENTER_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = pan_cen_y - height_obj/2 + padding_y;
        break;
    case TOP_CENTER:
        _x = pan_cen_x - width_obj/2 + padding_x;
        _y = (screen_coord) ? 0 + padding_y :
            height_pan - height_obj + padding_y;
        break;
    case BOTTOM_CENTER:
        _x = pan_cen_x - width_obj/2 + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y :
            0 + padding_y;
        break;
    case TOP_LEFT:
        _x = 0 + padding_x;
        _y = (screen_coord) ? 0 + padding_y :
            height_pan - height_obj + padding_y;
        break;
    case TOP_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = (screen_coord) ? 0 + padding_y :
            height_pan - height_obj + padding_y;
        break;
    case BOTTOM_LEFT:
        _x = 0 + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y :
            0 + padding_y;
        break;
    case BOTTOM_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y :
            0 + padding_y;
        break;
    \
    default:
        _x = _y = 0;  // For a warning (it doesn't recognize sceExitGame() closes
                      // the program) and says vals uninitialized
        assert(0, "Failed to match a pivot");
        break;
    }

    return Vector2d(_x, _y);
}

std::pair<int, int> Menu::add_component(Position _pos, Component comp,
    int padding_x /* = 0*/, int padding_y /* = 0*/
) {
    uint16_t comp_width;
    uint16_t comp_height;
    switch (comp.data.type) {
        case Component::IMAGE_TYPE:
            comp_width = comp.data.data.img.width;
            comp_height = comp.data.data.img.height;
            break;

        case Component::LABEL_TYPE:
            comp_width = strlen(comp.data.data.text) * get_font_width();
            comp_height =  get_font_height();
            break;

        case Component::PANEL_TYPE:
            comp_width = comp_width;
            comp_height = comp.height;
            break;

        default:
            comp_width = 0;
            comp_height = 0;
            assert(0, "type %d not recognized", comp.data.type);
            break;
    }

    if (comp_height > height || comp_width > width) {
        log(ERROR, "Attempt to add component (%d x %d) to canvas (%d x %d)",
            comp_width, comp_height, width, height);
    }

    int comp_index = components.size();
    int group_index = groups.size();

    Vector2d vec = pos_to_coord(_pos, comp.height, comp.width, height, width,
        false, padding_x, padding_y);

    comp.x = vec.x;
    comp.y = vec.y;

    components.push_back(comp);
    groups.push_back(GroupInfo(true, 1, 1, {&components[components.size()-1]}));

    return {comp_index, group_index};
}

std::pair<int, int> Menu::add_component_group(Position pos,
    std::vector<Component> arr, Grouping grouping, int spacing/* = 1*/,
    int padding_x/* = 0*/, int padding_y/* = 0*/, int rows/* = 0*/,
    int cols/* = 0*/, bool row_major/* = true*/
) {
    int first_comp_index = components.size();
    int group_index = groups.size();
    switch (grouping) {
    case VERTICAL_LIST:
        row_major = false;
        rows = arr.size();
        cols = 1;
        add_grid_col_major(pos, arr, spacing, padding_x, padding_y, rows, cols);
        break;

    case HORIZONTAL_LIST:
        row_major = true;
        rows = 1;
        cols = arr.size();
        add_grid_row_major(pos, arr, spacing, padding_x, padding_y, rows, cols);
        break;

    case GRID:
        if (row_major) {
            add_grid_row_major(pos, arr, spacing, padding_x, padding_y, rows,
                cols);
        } else {
            add_grid_col_major(pos, arr, spacing, padding_x, padding_y, rows,
                cols);
        }
        break;

    default:
        assert(0, "Failed to match grouping %d", grouping);
        break;
    }

    std::vector<Component *> group_vec;
    for (unsigned int i = first_comp_index; i < components.size(); i++) {
        group_vec.push_back(&components[i]);
    }
    groups.push_back(GroupInfo(row_major, rows, cols, group_vec));

    return {first_comp_index, group_index};
}

void Menu::add_grid_row_major(Position pos, std::vector<Component> arr,
    int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/
) {
    // Check if rows provided was correct
    if (ceil(arr.size() * 1.0f) / cols < rows) {
        log(ERROR, "Too many rows provided");
    }
    if (ceil(arr.size() * 1.0f) / cols > rows) {
        log(ERROR, "Not enough rows provided");
    }

    // Store the widest values of each row / column so that we can center
    // the smaller values
    int widest[cols] = {0};
    int tallest[rows] = {0};

    // Total dimensions of the grid
    unsigned int total_height = 0;
    unsigned int total_width = 0;

    // Determine the widest and tallest member of each column and row
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr[i].width > widest[i%cols]) widest[i%cols] = arr[i].width;
        if (arr[i].height > tallest[i/cols]) tallest[i/cols] = arr[i].height;
    }

    // Set the dimensions using the widest and tallest values of each row and
    // col
    for (int i=0; i < cols; i++) total_width += (widest[i] + spacing);
    for (int i=0; i < rows; i++) total_height += (tallest[i] + spacing);

    // Total spacing = (row/cols -1) * spacing
    total_height -= spacing;
    total_width -= spacing;

    // Get the coordinates of the grid from relative postion: pos
    Vector2d curr_coord = pos_to_coord(pos, total_height, total_width, height,
        width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    if (curr_coord.y + total_height >= height) {
        log(WARNING, "Grid height of %d exceeds max panel height of %d for "
            "coords %d, %d", total_height, height - curr_coord.y,
            curr_coord.x, curr_coord.y);
    }
    if (curr_coord.x + total_width >= width) {
        log(WARNING, "Grid width of %d exceeds max panel width of %d for "
            "coords %d, %d", total_width, width - curr_coord.x, curr_coord.x,
            curr_coord.y);
    }

    // We draw components top down left to right
    curr_coord.y += total_height - tallest[0];
    int start_x = curr_coord.x;

    for (unsigned int i = 0; i < arr.size(); i++) {
        if (i % cols == 0 && i > 0) {  // next row
            curr_coord.y -= (tallest[i/cols] + spacing);
            curr_coord.x = start_x;
        }

        arr[i].x = curr_coord.x;
        arr[i].y = curr_coord.y;

        // Put in the center of the row/col
        arr[i].y += (tallest[i/cols] - arr[i].height)/2;
        arr[i].x += (widest[i%cols] - arr[i].width)/2;

        // Move for next element
        curr_coord.x += (widest[i%cols] + spacing);

        components.push_back(arr[i]);
    }
}

void Menu::add_grid_col_major(Position pos, std::vector<Component> arr,
    int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/
) {
    // Check if cols provided was correct
    if (ceil(arr.size() * 1.0f / rows) < cols) {
        log(ERROR, "Too many columns provided");
    }
    if (ceil(arr.size() * 1.0f / rows) > cols) {
        log(ERROR, "Not enough columns provided");
    }

    // Store the widest values of each row / column so that we can center
    // the smaller values
    int widest[cols] = {0};
    int tallest[rows] = {0};

    // Total dimensions of the grid
    unsigned int total_height = 0;
    unsigned int total_width = 0;

    // Determine the widest and tallest member of each column and row
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr[i].width > widest[i/rows]) widest[i/rows] = arr[i].width;
        if (arr[i].height > tallest[i%rows]) tallest[i%rows] = arr[i].height;
    }

    // Set the dimensions using the widest and tallest values of each row
    // and col
    for (int i=0; i < cols; i++) total_width += (widest[i] + spacing);
    for (int i=0; i < rows; i++) total_height += (tallest[i] + spacing);

    for (int i=0; i < cols; i++) log(DEBUG, "widest %d, %d", widest[i], i);
    for (int i=0; i < rows; i++) log(DEBUG, "tallest %d, %d", tallest[i], i);

    // Total spacing = (row/cols -1) * spacing
    total_height -= spacing;
    total_width -= spacing;

    // Get the coordinates of the grid from relative postion: pos
    Vector2d curr_coord = pos_to_coord(pos, total_height, total_width, height,
        width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    if (curr_coord.y + total_height >= height) {
        log(WARNING, "Grid height of %d exceeds max panel height of %d for "
            "coords %d, %d", total_height, height - curr_coord.y,
            curr_coord.x, curr_coord.y);
    }
    if (curr_coord.x + total_width >= width) {
        log(WARNING, "Grid width of %d exceeds max panel width of %d for "
            "coords %d, %d", total_width, width - curr_coord.x, curr_coord.x,
            curr_coord.y);
    }

    // We draw components top down left to right
    curr_coord.y += total_height;
    int start_y = curr_coord.y;
    for (unsigned int i = 0; i < arr.size(); i++) {
        // Move for next element
        if (i % rows == 0 && i > 0) {  // next col
            curr_coord.x += (widest[(i-1)/rows] + spacing);
            curr_coord.y = start_y;
        }

        arr[i].x = curr_coord.x;
        arr[i].y = curr_coord.y;

        // Put in the center of the row/col
        arr[i].y -= (tallest[i%rows] + arr[i].height)/2;
        arr[i].x += (widest[i/rows] - arr[i].width)/2;

        curr_coord.y -= (tallest[i%rows] + spacing);
        components.push_back(arr[i]);
    }
}

void Menu::draw_panel(Component comp) {
    assert(comp.data.type == Component::PANEL_TYPE, "");

    switch (comp.data.data.shape) {
    case Component::Rectangle:
        for (int y = comp.y; y < comp.height + comp.y; y++) {
            for (int x = comp.x; x < comp.width + comp.x; x++) {
                gui.img_matrix[width*y + x] =
                    highlight_selection(comp, comp.background_color);
            }
        }
        break;

    case Component::Circle:
        int _x;
        int _y;
        for (int y = comp.y; y < comp.height + comp.y; y++) {
            for (int x = comp.x; x < comp.width + comp.x; x++) {
                _x = comp.x - x + comp.width/2;
                _y = comp.y - y + comp.height/2;

                // We know that the general equation for a circle is
                // ( x - h )^2 + ( y - k )^2 = r^2, where ( h, k ) is the center
                // and r is the radius.
                if((pow(_x, 2) + pow(_y, 2)) <= pow(comp.width/2, 2)) {
                    gui.img_matrix[width*y + x] =
                        highlight_selection(comp, comp.background_color);
                }
            }
        }
        break;

    default:
        break;
    }
}

void Menu::draw_text(Component comp) {
    assert(comp.data.type == Component::LABEL_TYPE, "");
    // Create temporary component object
    Component temp = comp;
    temp.data.type = Component::IMAGE_TYPE;
    temp.data.data.img = text(comp.data.data.text); // assign image

    draw_img(temp);

    psp_free(temp.data.data.img.img_matrix);
}

void Menu::draw_img(Component comp) {
    assert(comp.data.type == Component::IMAGE_TYPE, "");

    Image _img = comp.data.data.img;
    for (unsigned int y = comp.y; y < _img.height + comp.y; y++) {
        for (unsigned int x = comp.x; x < _img.width + comp.x; x++) {
            if (y < height && x < width) {
                if (GFX::is_transparent(_img.img_matrix[_img.width*(y - comp.y) + (x - comp.x)])){
                    if (comp.background_color ) {
                        gui.img_matrix[width*y + x] =
                            highlight_selection(comp, comp.background_color);
                    }
                }
                else {
                    gui.img_matrix[width*y + x] =
                        highlight_selection(comp, _img.img_matrix[_img.width*(y - comp.y) + (x - comp.x)]);
                }
            }
        }
    }
}

Menu Menu::set_pos(Position pos, int padding_x/* = 0*/, int padding_y/* = 0*/) {
    Vector2d vec = pos_to_coord(pos, height, width, SCREEN_HEIGHT,
        SCREEN_WIDTH_RES, true, padding_x, padding_y);

    x = vec.x;
    y = vec.y;
    return *this;
}

void Menu::update() {
    for (unsigned int i = 0; i < width * height; i++) {
        gui.img_matrix[i] = background_color;
    }
    for (Component comp: components) {
        if (comp.hidden) continue;

        switch (comp.data.type) {
            case Component::IMAGE_TYPE:
                draw_img(comp);
                break;

            case Component::LABEL_TYPE:
                draw_text(comp);
                break;

            case Component::PANEL_TYPE:
                draw_panel(comp);
                break;

            default:
                assert(0, "type %d not recognized", comp.data.type );
                break;
        }
    }
}

void Menu::draw_and_swap_buffers() {
    GFX::simple_drawBMP(x, y, gui);
    GFX::swapBuffers();
}

void Menu::close() {
    log(DEBUG, "Closing menu: %s", menu_name);
    for (Component comp : components) {
        if (comp.data.type == Component::IMAGE_TYPE) {
            psp_free(comp.data.data.img.img_matrix);
        }
    }
    psp_free(gui.img_matrix);
}

void Menu::set_cursor_position() {
    int rows = groups[selected_group].rows;
    int cols = groups[selected_group].cols;
    bool row_major = groups[selected_group].row_major;

    cursor_position = 0;

    if (row_major) {
        if (selected_comp/cols == rows-1) {
            // Bitwise or then assignment
            cursor_position |= LAST_ROW;
        }
        if (selected_comp/cols == 0) {
            cursor_position |= FIRST_ROW;
        }
        if (selected_comp%cols ==cols-1) {
            cursor_position |= LAST_COL;
        }
        if (selected_comp%cols == 0) {
            cursor_position |= FIRST_COL;
        }
    } else {
        if (selected_comp%rows == rows-1) {
            cursor_position |= LAST_ROW;
        }
        if (selected_comp%rows == 0) {
            cursor_position |= FIRST_ROW;
        }
        if (selected_comp/cols ==cols-1) {
            cursor_position |= LAST_COL;
        }
        if (selected_comp/cols == 0) {
            cursor_position |= FIRST_COL;
        }
    }
}

std::vector<int> Menu::get_selectable_components(std::vector<Component *> arr) {
    std::vector<int> selectable_arr;
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr[i]->selectable) selectable_arr.push_back(i);
    }

    return selectable_arr;
}

int Menu::count_selectable_components(int group){
    if (group > groups.size()-1) return 0;

    return get_selectable_components(groups[group].components).size();
}

void Menu::set_selection_group(int group_index) {
    if (!count_selectable_components(group_index)) {
        log(WARNING, "Choosen group %d has not selectable components"
            , group_index);
    }
    selected_group = group_index;
    selected_comp = 0;
    set_cursor_position();
    groups[selected_group].components[0]->select();
}

int Menu::set_selection_polarity(int pol){
    Component * comp = get_selected_component();

    if (!comp) {
        return -1;
    }

    if (pol) {
        comp->select();
    } else {
        comp->deselect();
    }

    return 1;
}

void Menu::select_next(Direction direction) {
    set_selection_polarity(0);  // Deselect the old item

    int rows = groups[selected_group].rows;
    int cols = groups[selected_group].cols;
    int size = count_selectable_components(selected_group);

    switch (direction)
    {
    case UP:
        if (rows <= 1) break;

        if (groups[selected_group].row_major) {
            selected_comp -= cols;  // Go to upper row
            // If that is out of bounds add the max grid value to move to the
            // bottom of the grid, if the grid is not complete this is out of
            // bounds
            if (selected_comp < 0) selected_comp += (rows * cols);

            // Move selection in bounds
            if (selected_comp >= size) selected_comp = size-1;
        }
        else { // Column Major
            // If current position is the first element...
            if (selected_comp % rows == 0) {
                // then to move up we wrap around to the end of the column...
                selected_comp += rows - 1;
                // bounds checking if the grid is not complete
                if (selected_comp >= size) selected_comp = size-1;
            } else {
                // Otherwise just decrement
                selected_comp--;
            }
        }

        break;
    case DOWN:
        if (rows <= 1) break;

        if (groups[selected_group].row_major) {
            selected_comp += cols;  // Go to lower row

            // If that is out of bounds subtract the max grid value to move to
            //the top of the grid, if the grid is not complete this is out of
            // bounds
            if (selected_comp >= size) selected_comp %= (rows * cols);

            // Move selection in bounds
            if (selected_comp >= size) selected_comp = size-1;
        }
        else { // Column major
            // If current position is the last element...
            if (selected_comp % rows == rows - 1) {
                // then to move down we wrap around to the end of the row...
                selected_comp -= rows - 1;
            } else {
                // Other wise just increment
                selected_comp++;
                // bounds checking if the grid is not complete
                if (selected_comp >= size) selected_comp = rows * (size/rows);
            }
        }
        break;
    case LEFT:
        if (groups[selected_group].cols <= 1) break;

        if (groups[selected_group].row_major) {
            // If current position is the first element...
            if (selected_comp % cols == 0) {
                // then to move left we wrap around to the back of the row...
                selected_comp += cols - 1;
                // bounds checking if the grid is not complete
                if (selected_comp >= size) selected_comp = size-1;
            } else {
                // Other wise just decrement
                selected_comp--;
            }
        }
        else {
            selected_comp -= rows;  // Go to upper column
            // If that is out of bounds add the max grid value to move to the
            // bottom of the grid, if the grid is not complete this is out of
            // bounds
            if (selected_comp < 0) selected_comp += (rows * cols);

            // Move selection in bounds
            if (selected_comp >= size) selected_comp = size-1;
        }

        break;
    case RIGHT:
        if (groups[selected_group].cols <= 1) break;

        if (groups[selected_group].row_major) {
            // If current position is the last element...
            if (selected_comp % cols == cols - 1) {
                // then to move right we wrap around to the front of the row...
                selected_comp -= cols - 1;
            } else {
                // Other wise just increment
                selected_comp++;
                // bounds checking if the grid is not complete
                if (selected_comp >= size) selected_comp = cols * (size/cols);
            }
        } else {
            selected_comp += rows;  // Go to next column

            // If that is out of bounds subtract the max grid value to move to
            // the top of the grid, if the grid is not complete this is out of
            // bounds
            if (selected_comp >= size) selected_comp %= (rows * cols);

            // Move selection in bounds
            if (selected_comp >= size) selected_comp = size-1;
        }

        break;

    default:
        assert(0, "value %d did not match a valid direction", direction);
        break;
    }
    log(DEBUG, "cur pos: %d", cursor_position);
    log(DEBUG, "selected: %d", selected_comp);
    log(DEBUG, "size: %d", groups[selected_group].components.size());
    log(DEBUG, "rows: %d cols: %d", rows, cols);

    set_cursor_position();
    set_selection_polarity(1);  // Select the component
}

void Menu::next_group() {
    set_selection_polarity(0);  // Deselect old select
    do {
        selected_group =
            (selected_group + 1 < groups.size()) ? selected_group + 1 : 0;
        log(DEBUG, "Moving to next group");
    } while (!count_selectable_components(selected_group));

    selected_comp = 0;
    set_cursor_position();
    set_selection_polarity(1);
}

void Menu::prev_group() {
    set_selection_polarity(0);  // Deselect old select
    int num_selectable = 0;
    do {
        selected_group =
            (selected_group - 1 > -1) ? selected_group - 1 : groups.size() - 1;
        log(DEBUG, "Moving to prev group");

        num_selectable = count_selectable_components(selected_group);
    } while (!num_selectable);

    selected_comp = num_selectable - 1;
    set_cursor_position();
    set_selection_polarity(1);
}

Component * Menu::get_component(int index) {
    if (index < 0 || index > components.size()) {
        log(WARNING, "index of %d passed to get_component is out of bounds",
            index);
        return NULL;
    }

    return &components[index];
}

Component * Menu::get_selected_component() {
    if (components.size() == 0 || groups.size() == 0) {
        log(WARNING, "Groups and / or component list is empty");
        return NULL;
    }

    std::vector<int> selectable =
        get_selectable_components(groups[selected_group].components);

    if (selectable.size() == 0) {
        log(WARNING, "No selections for group %d group size %d", selected_group,
            groups[selected_group].components.size());
        return NULL;
    }

    int index = selectable[selected_comp];

    return groups[selected_group].components[index];
}

void Menu::click_selection() {
    // Get the array of selectable buttons within a group
    std::vector<int> selectable =
        get_selectable_components(groups[selected_group].components);

    if (selectable.size() == 0) {
        log(WARNING, "No selections for group %d group size %d", selected_group,
            groups[selected_group].components.size());
        return;
    }

    // Get the group array index of the item we wish to deselect
    int index = selectable[selected_comp];
    groups[selected_group].components[index]->on_click(); // do the on_click function
}