#include "component.h"

Component::Component(int _x, int _y, int _width, int _height, DataUnion dat, 
    uint32_t _color/* = 0x00000000*/
){
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    background_color = _color;
}

Component::~Component()
{
}