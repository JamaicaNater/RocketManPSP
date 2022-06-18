#include "vector2d.h"

Vector2d::Vector2d() {
    x = y = angle = 0;
}

Vector2d::Vector2d(short _x, short _y){
    x = _x;
    y = _y;
    angle = 0;
}

void Vector2d::set_angle(short _angle) {
    // if (_angle > 360) _angle%=360;
    // if (_angle < 360) _angle%=-360;
    angle = _angle;
}

short Vector2d::get_angle() {
    return angle;
}

short Vector2d::get_mirrored_angle (){
    short mirrored_angle = (angle < 0)? -180 - angle: 180 - angle;
    return mirrored_angle;
}