#include "../image/image.h"

class Component
{
private:
    enum Shape {Rectangle, Circle, Triangle};
    enum ComponentType{PANEL, LABEL, EXTERN_IMG};

    typedef union
    {
        Shape shape;
        Image img;
        char text[64];
    } DataUnion;

    int x, y, width, height;
    uint32_t background_color;
    
public:
    Component(int _x, int _y, int _width, int _height, DataUnion _dat, uint32_t _color = 0x00000000);
    ~Component();
};
