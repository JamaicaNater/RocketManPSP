#include "menu.h"

Menu::Menu(unsigned int _x, unsigned int _y, unsigned int _height, 
    unsigned int _width, uint32_t _color
){
    x = _x;
    y = _y;
    height = _height;
    width = _width;

    img_matrix = (unsigned int *)psp_malloc(width * height * sizeof(unsigned int));
    for (unsigned int i = 0; i < width * height; i++) img_matrix[i] = _color;
}

Menu::Menu(pivots _pos, unsigned int _height, unsigned int _width, 
        uint32_t _color, int padding_x/* = 0*/, int padding_y/* = 0*/
){
    Vector2d vec = pivot_to_coord(_pos, _height, _width, SCREEN_HEIGHT, 
        SCREEN_WIDTH_RES, true, padding_x, padding_y);

    x = vec.x;
    y = vec.y;
    height = _height;
    width = _width;

    img_matrix = (unsigned int *)psp_malloc(width * height * sizeof(unsigned int));
    for (unsigned int i = 0; i < width * height; i++) img_matrix[i] = _color;
}

Menu::~Menu(){
//TODO DFS style dealloc
}

Vector2d Menu::pivot_to_coord(pivots pos, unsigned int height_obj, 
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
        PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to match a pivot"
            "");
        break;
    }

    return Vector2d(_x,_y);
}

void Menu::add_component(pivots _pos, Component comp, int padding_x /* = 0*/, 
    int padding_y /* = 0*/
){
    Vector2d vec = pivot_to_coord(_pos, comp.height, comp.width, height, width, 
        false, padding_x, padding_y);

    if (comp.data.type == Component::IMAGE_TYPE) {
        add_img(vec.x, vec.y, comp.data.data.img);
    }
    if (comp.data.type == Component::LABEL_TYPE) {
        
    }
    if (comp.data.type == Component::PANEL_TYPE) {
        add_panel(vec.x, vec.y, comp);
    }
}

void Menu::add_panel(unsigned int _x, unsigned int _y, Component comp){
    for (unsigned int y = _y; y < comp.height + _y; y++){
        for (unsigned int x = _x; x < comp.width + _x; x++) {
            img_matrix[width*y + x] = comp.background_color;
        }
    }
}

void Menu::add_img(unsigned int _x, unsigned int _y, Image _img){
    for (unsigned int y = _y; y < _img.height + _y; y++){
        for (unsigned int x = _x; x < _img.width + _x; x++) {
            if (!GFX::is_transparent(_img.img_matrix[_img.width*(y - _y) + (x - _x)]) 
                && y < height && x < width
            ) {
                img_matrix[width*y + x] = _img.img_matrix[_img.width*(y - _y) + (x - _x)];
            }
        }
    }
}

void Menu::set_pos(pivots pos, int padding_x /* = 0*/, int padding_y /* = 0*/){
    Vector2d vec  = pivot_to_coord(pos, height, width, SCREEN_HEIGHT, 
        SCREEN_WIDTH_RES, true, padding_x, padding_y);

    x = vec.x;
    y = vec.y;
}

Image Menu::get_image() {
    return Image(height, width, img_matrix, "Menu");
}