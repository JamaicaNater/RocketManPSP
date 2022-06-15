#pragma once

#include "../image/image.h"

class Component
{
public:
    enum Shape {Rectangle, Circle, Triangle};
    enum ComponentType{NONE_TYPE, PANEL_TYPE, LABEL_TYPE, IMAGE_TYPE};

    struct CompData
    {
        typedef union D_U
        {
            Shape shape;
            Image img;
            char text[64];

            D_U(){}
            ~D_U(){}
        } DataUnion;


        ComponentType type;
        DataUnion data;

        CompData() : type (NONE_TYPE){

        }
        CompData(char t[]) : type(LABEL_TYPE){
            strncpy(data.text, t, 63);
        }
        CompData(Image i) : type(IMAGE_TYPE){
            data.img = i;
        }
        CompData(Shape s) : type(PANEL_TYPE){
            data.shape = s;
        }
    };
    
    int x,y;
    int width = 0, height = 0;
    uint32_t background_color = 0x00000000;
    CompData data;
    
public:
    Component(int _height, int _width, CompData _dat, uint32_t _color = 0x00000000);
    ~Component();
};
