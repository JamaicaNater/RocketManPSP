#pragma once

#include "../image/image.h"
#include <stdint.h>
#include <functional>

class Component
{
public:
    enum Shape {Rectangle, Circle};
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
    };
    
    int x,y;
    int width = 0, height = 0;
    uint32_t background_color = 0x00000000;
    CompData data;

    bool selectable = true, selected = false, hidden = false;

    std::function<void()> on_click;
    std::function<void()> on_select;
        
    Component(int _height, int _width, Shape s, uint32_t _color = 0x00000000);
    Component(const char * txt, uint32_t _color = 0x00000000);
    Component(Image _img, uint32_t _color = 0x00000000);
    ~Component();

    void set_x(int _x);
    void set_y(int _y);

    Component hide();
    Component show();

    Component select();
    Component deselect();

    Component set_selectable(bool _selectable);
};
