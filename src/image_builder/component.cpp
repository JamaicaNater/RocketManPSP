#include "component.h"

Component::Component(int _height, int _width, CompData _dat, uint32_t _color/* = 0x00000000*/
){
    width = _width;
    height = _height;
    background_color = _color;
    
    data.data = _dat.data;
    data.type = _dat.type;
}

Component::~Component()
{
}