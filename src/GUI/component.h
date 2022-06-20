#pragma once

#include "image/image.h"
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

    /**
     * @brief Set the x postion of the object
     *
     * @param _x
     * @return Component
     */
    Component set_x(int _x);

    /**
     * @brief Set the y postion of the object
     *
     * @param _y
     * @return Component
     */
    Component set_y(int _y);

    /**
     * @brief hides the object
     *
     * @return Component
     */
    Component hide();
    /**
     * @brief unhides the object
     *
     * @return Component
     */
    Component show();

    /**
     * @brief Selects (highlights) the object
     *
     * @return Component
     */
    Component select();

    /**
     * @brief Deselects the object
     *
     * @return Component
     */
    Component deselect();

    /**
     * @brief Set is the object is selectable
     *
     * @param _selectable
     * @return Component
     */
    Component set_selectable(bool _selectable);

    /**
     * @brief Set what do to do if the user clicks the component
     *
     */
    Component set_on_click(std::function<void()> _on_click);

    void log_info();
};
