#include "menu.h"

#include <math.h>

#include "text_builder.h"

Menu::Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
    unsigned int _width, uint32_t _color) 
        : gui(_height, _width, 
            (uint32_t *)psp_malloc(_width * _height * sizeof(uint32_t)), "menu"
) {
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
){
    Vector2d vec = pivot_to_coord(_pos, _height, _width, SCREEN_HEIGHT, 
        SCREEN_WIDTH_RES, true, padding_x, padding_y);

    x = vec.x;
    y = vec.y;
    height = _height;
    width = _width;
    background_color = _color;    
}

Menu::~Menu(){
//TODO DFS style dealloc
}

Vector2d Menu::pivot_to_coord(Position pos, unsigned int height_obj, 
    unsigned int width_obj, unsigned int height_pan, unsigned int width_pan,
    bool screen_coord, int padding_x/* = 0*/, int padding_y/* = 0*/
){
    int menu_cen_x = width_pan/2;
    int menu_cen_y = height_pan/2;

    int _x,_y;

    switch (pos) {
    case CENTER:
        _x = menu_cen_x - width_obj/2 + padding_x;
        _y = menu_cen_y - height_obj/2 + padding_y;
        break;
    case CENTER_LEFT:
        _x = 0 + padding_x;
        _y = menu_cen_y - height_obj/2 + padding_y;
        break;
    case CENTER_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = menu_cen_y - height_obj/2 + padding_y;
        break;
    case TOP_CENTER:
        _x = menu_cen_x - width_obj/2 + padding_x;
        _y = (screen_coord) ? 0 + padding_y : height_pan - height_obj + padding_y;
        break;
    case BOTTOM_CENTER:
        _x = menu_cen_x - width_obj/2 + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y: 0 + padding_y;
        break;
    case TOP_LEFT:
        _x = 0 + padding_x;
        _y = (screen_coord) ? 0 + padding_y : height_pan - height_obj + padding_y;
        break;
    case TOP_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = (screen_coord) ? 0 + padding_y : height_pan - height_obj + padding_y;
        break;
    case BOTTOM_LEFT:
        _x = 0 + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y: 0 + padding_y;
        break;
    case BOTTOM_RIGHT:
        _x = width_pan - width_obj + padding_x;
        _y = (screen_coord) ? height_pan - height_obj + padding_y: 0 + padding_y;
        break;
    \
    default:
        _x = _y = 0;
        log(CRITICAL, "Failed to match a pivot"
            "");
        break;
    }

    return Vector2d(_x,_y);
}

void Menu::add_component(Position _pos, Component comp, int padding_x /* = 0*/, 
    int padding_y /* = 0*/
){
    Vector2d vec = pivot_to_coord(_pos, comp.height, comp.width, height, width, 
        false, padding_x, padding_y);
    comp.x = vec.x;
    comp.y = vec.y;

    components.push_back(comp);
}

void Menu::add_component_group(Position pos, std::vector<Component> arr, 
    Grouping grouping, int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/, bool row_major/* = true*/
) {
    switch (grouping)
    {
    case VERTICAL_LIST:
        add_grid_row_major(pos, arr, spacing, padding_x, padding_y, arr.size(), 1);
        break;

    case HORIZONTAL_LIST:
        add_grid_row_major(pos, arr, spacing, padding_x, padding_y, 1, arr.size());
        break;
    
    case GRID:
        if(row_major) add_grid_row_major(pos, arr, spacing, padding_x,
            padding_y, rows, cols);
        else add_grid_col_major(pos, arr, spacing, padding_x, padding_y, 
            rows, cols);
        break;
    
    default:
        break;
    }
}

void Menu::add_grid_row_major(Position pos, std::vector<Component> arr,
    int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/
) {
    int widest[cols] = {0};
    int tallest[rows] = {0};

    int total_height = 0;
    int total_width = 0;
    
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr[i].width > widest[i%cols]) widest[i%cols] = arr[i].width;
        if (arr[i].height > tallest[i/cols]) tallest[i/cols] = arr[i].height;
    }

    for (int i=0; i < cols; i++) total_width += (widest[i] + spacing);
    for (int i=0; i < rows; i++) total_height += (tallest[i] + spacing);
    total_height -= spacing;
    total_width -= spacing;

    Vector2d curr_coord = pivot_to_coord(pos, total_height, total_width, height, width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    curr_coord.y += total_height - tallest[0]; // top down left right
    int start_x = curr_coord.x;
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (i % cols == 0 && i > 0) { // next row
            curr_coord.y -= tallest[i/cols] + spacing;
            curr_coord.x = start_x;
        }

        arr[i].x = curr_coord.x;
        arr[i].y = curr_coord.y;

        arr[i].y += (tallest[i/cols] - arr[i].height)/2;
        arr[i].x += (widest[i%cols] - arr[i].width)/2;
        
        curr_coord.x += widest[i%cols] + spacing;

        components.push_back(arr[i]);
    }
}

void Menu::add_grid_col_major(Position pos, std::vector<Component> arr, 
    int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/
) {
    int widest[rows] = {0};
    int tallest[cols] = {0};

    int total_height = 0;
    int total_width = 0;
    
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr[i].width > widest[i/rows]) widest[i/rows] = arr[i].width;
        if (arr[i].height > tallest[i%rows]) tallest[i%rows] = arr[i].height;
    }

    for (int i=0; i < cols; i++) total_width += (widest[i] + spacing);
    for (int i=0; i < rows; i++) total_height += (tallest[i] + spacing);
    total_height -= spacing;
    total_width -= spacing;

    Vector2d curr_coord = pivot_to_coord(pos, total_height, total_width, height, width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    curr_coord.y += total_height - tallest[0]; // top down left right
    int start_y = curr_coord.y;
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (i % rows == 0 && i > 0) { // next row
            curr_coord.x += widest[i%rows] + spacing;
            curr_coord.y = start_y;
        }

        arr[i].x = curr_coord.x;
        arr[i].y = curr_coord.y;

        arr[i].y -= (tallest[i%rows] - arr[i].height)/2;
        arr[i].x += (widest[i/rows] - arr[i].width)/2;
        
        curr_coord.y -= tallest[i/rows] + spacing;

        components.push_back(arr[i]);
    }
}

void Menu::draw_panel(Component comp){
    switch (comp.data.data.shape)
    {
    case Component::Rectangle:
        for (int y = comp.y; y < comp.height + comp.y; y++){
            for (int x = comp.x; x < comp.width + comp.x; x++) {
                gui.img_matrix[width*y + x] = comp.background_color;
            }
        }
        break;

    case Component::Circle:
        int _x;
        int _y;
        for (int y = comp.y; y < comp.height + comp.y; y++){
            for (int x = comp.x; x < comp.width + comp.x; x++) {
                _x = comp.x - x + comp.width/2;
                _y = comp.y - y + comp.height/2;

                // We know that the general equation for a circle is 
                // ( x - h )^2 + ( y - k )^2 = r^2, where ( h, k ) is the center 
                // and r is the radius.
                if((pow(_x,2) + pow(_y,2)) <= pow(comp.width/2,2)) {
                    gui.img_matrix[width*y + x] = comp.background_color;  
                } 
            }
        }
        break;
    
    default:
        break;
    }
}

void Menu::draw_img(Component comp){
    Image _img = comp.data.data.img;
    for (unsigned int y = comp.y; y < _img.height + comp.y; y++){
        for (unsigned int x = comp.x; x < _img.width + comp.x; x++) {
            if (y < height && x < width) {
                if (GFX::is_transparent(_img.img_matrix[_img.width*(y - comp.y) + (x - comp.x)])){
                    if (comp.background_color ) gui.img_matrix[width*y + x] = comp.background_color;
                }
                else gui.img_matrix[width*y + x] = _img.img_matrix[_img.width*(y - comp.y) + (x - comp.x)];
            }
        }
    }
}

void Menu::set_pos(Position pos, int padding_x /* = 0*/, int padding_y /* = 0*/){
    Vector2d vec  = pivot_to_coord(pos, height, width, SCREEN_HEIGHT, 
        SCREEN_WIDTH_RES, true, padding_x, padding_y);

    x = vec.x;
    y = vec.y;
}

void Menu::update(){   
    for (unsigned int i = 0; i < width * height; i++) gui.img_matrix[i] = background_color; 
    for (Component comp: components){
        if (comp.data.type == Component::IMAGE_TYPE) {
            draw_img(comp);
        }
        if (comp.data.type == Component::LABEL_TYPE) {
            //TODO simplify this
            Image img = text(comp.data.data.text); // Todo: data.data?
            Component temp = Component(img);
            temp.set_x(comp.x);
            temp.set_y(comp.y);
            temp.background_color = comp.background_color;
            draw_img(temp);
            free(img.img_matrix);
        }
        if (comp.data.type == Component::PANEL_TYPE) {
            draw_panel(comp);
        }
    }
}