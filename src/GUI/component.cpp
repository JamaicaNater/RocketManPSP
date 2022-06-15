#include "component.h"
#include "text_builder.h"

Component::Component(int _height, int _width, CompData _dat, uint32_t _color/* = 0x00000000*/
){
    width = _width;
    height = _height;
    background_color = _color;
    
    data.data = _dat.data;
    data.type = _dat.type;
}

Component::Component(int _height, int _width, Shape s, uint32_t _color/* = 0x00000000*/
){
    width = _width;
    height = _height;
    background_color = _color;
    
    data.data.shape = s;
    data.type = PANEL_TYPE;
}

Component::Component(const char * txt, uint32_t _color/* = 0x00000000*/){
    width = get_font_width() * strlen(txt);
    height = get_font_height();

    background_color = _color;
    
    strncpy(data.data.text, txt, 63);
    data.type = LABEL_TYPE;
}

Component::Component(Image _img, uint32_t _color/* = 0x00000000*/){
    width = _img.width;
    height = _img.height;
    background_color = _color;
    
    data.data.img = _img; 
    data.type = IMAGE_TYPE;
}

Component::~Component()
{
}

void Component::set_x(int _x){
    x = _x;
}

void Component::set_y(int _y){
    y = _y;
}