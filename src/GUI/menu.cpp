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

Menu::Menu(pivots _pos, unsigned int _height, unsigned int _width, 
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
    comp.x = vec.x;
    comp.y = vec.y;

    components.push_back(comp);
}

void Menu::add_component_group(pivots pos, std::vector<Component> arr, 
    Grouping grouping, int spacing/* = 1*/, int padding_x/* = 0*/, int padding_y/* = 0*/,
    int rows/* = 0*/, int cols/* = 0*/
) {
    switch (grouping)
    {
    case VERTICAL_LIST:
        add_vertical_list(pos, arr, spacing, padding_x, padding_y);
        break;

    case HORIZONTAL_LIST:
        add_horizontal_list(pos, arr, spacing, padding_x, padding_y);
        break;
    
    case GRID:

        break;
    
    default:
        break;
    }
}

void Menu::add_vertical_list(pivots pos, std::vector<Component> arr, int spacing/* = 1*/, 
    int padding_x/* = 0*/, int padding_y/* = 0*/
) {
    int total_width = 0;
    int total_height = 0;
    // When implementing the grid will need to determwnt the talest widest part 
    // of each row and coloms and equalize
    for (Component &comp : arr){
        total_height += comp.height;
        total_width = (comp.width > total_width) ? comp.width : total_width;
    }

    total_height += spacing * (arr.size() - 1);

    Vector2d curr_coord = pivot_to_coord(pos, total_height, total_width, height, width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    for (Component &comp : arr){
        comp.x = curr_coord.x;
        comp.y = curr_coord.y;

        comp.x += (total_width - comp.width)/2;
        curr_coord.y += comp.height + spacing;
        
        components.push_back(comp);
    }
}
void Menu::add_horizontal_list(pivots pos, std::vector<Component> arr, int spacing/* = 1*/,
    int padding_x/* = 0*/, int padding_y/* = 0*/
) {
    int total_width = 0;
    int total_height = 0;

    for (Component &comp : arr){
        total_width += comp.width;
        total_height = (comp.height > total_height)? comp.height : total_height;
    }

    total_width += spacing * (arr.size() - 1);

    Vector2d curr_coord = pivot_to_coord(pos, total_height, total_width, height, width, false);
    curr_coord.x += padding_x;
    curr_coord.y += padding_y;

    for (Component &comp : arr){
        comp.x = curr_coord.x;
        comp.y = curr_coord.y;

        comp.y += (total_height - comp.height)/2;
        curr_coord.x += comp.width + spacing;
        components.push_back(comp);
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

void Menu::set_pos(pivots pos, int padding_x /* = 0*/, int padding_y /* = 0*/){
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