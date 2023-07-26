#include "physics/vector2d.h"

Vector2d::Vector2d() {
    x = y = angle = 0;
}

Vector2d::Vector2d(int16_t _x, int16_t _y) {
    x = _x;
    y = _y;
    angle = 0;
}

int16_t Vector2d::get_mirrored_angle() {
    int16_t mirrored_angle = (angle < 0)? -180 - angle: 180 - angle;
    return mirrored_angle;
}
