#include "GUI/component.h"
#include "GUI/text_builder.h"

char comp_types[4][11] = {"NONE_TYPE", "PANEL_TYPE", "LABEL_TYPE", "IMAGE_TYPE"};

Component::Component(int _height, int _width, Shape s, uint32_t _color/* = 0x00000000*/
){
    width = _width;
    height = _height;
    background_color = _color;

    data.data.shape = s;
    data.type = PANEL_TYPE;
}

Component::Component(const char * txt, uint32_t _color/* = 0x00000000*/) {
    width = get_font_width() * strlen(txt);
    height = get_font_height();

    background_color = _color;

    strncpy(data.data.text, txt, 63);
    data.type = LABEL_TYPE;
}

Component::Component(Image _img, uint32_t _color/* = 0x00000000*/) {
    width = _img.width;
    height = _img.height;
    background_color = _color;

    data.data.img = _img;
    data.type = IMAGE_TYPE;
}

Component::Component() {
}

Component::~Component() {
}

Component Component::set_x(int _x){
    x = _x;
    return *this;
}

Component Component::set_y(int _y){
    y = _y;
    return *this;
}

Component Component::hide() {
    hidden = true;
    return *this;
}

Component Component::show() {
    hidden = false;
    return *this;
}

Component Component::select() {
    if (selectable) selected = true;
    else log(WARNING, "Select called on non selectable object");
    return *this;
}
Component Component::deselect(){
    selected = false;
    return *this;
}

Component Component::set_selectable(bool _selectable){
    selectable = _selectable;
    return *this;
}

Component Component::set_on_click(std::function<void()> _on_click){
    on_click = _on_click;
    return *this;
}

void Component::log_info(){
    log(INFO, "x: %d y: %d, width: %d height %d, color %0x, type %d, "
        "selectable %d, selected %d, hidden %d ptr %0x", x, y, width, height,
        background_color, data.type, selectable, selected, hidden, this);
}