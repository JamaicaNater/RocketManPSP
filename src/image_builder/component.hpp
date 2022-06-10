#pragma once

#include <cstdlib>
#include "../image.hpp"
#include "../logger/logger.h"


struct Component
{
private:
    enum ComponentType{PANEL, BUTTON, LABEL, EXTERN_IMG};
    
    short x, y, height, width;
    char * text_field;
    //Component * children[4];

    unsigned int * img_matrix;
public:
    Component(short _x, short _y, short _height, short _width, unsigned int color){
        x = _x;
        y = _y;
        height = _height;
        width = _width;

        img_matrix = (unsigned int *)malloc(width * height * sizeof(unsigned int));
        for (int i = 0; i < width * height; i++) img_matrix[i] = color;
    }
    ~Component(){
    //TODO DFS style dealloc
    }

    void add_panel(short _x, short _y, short _height, short _width, unsigned int _color){
        for (short y = _x; y < _height + _y; y++){
            for (short x = _y; x < _width + _x; x++) img_matrix[width*y + x] = _color;
        }
    }

    Image get_image() {
        return Image(height, width, img_matrix, "component");
    }
};
