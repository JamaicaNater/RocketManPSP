#include "physics/vector2d.h"
#include <iostream>

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

char* Vector2d::to_string() const {
    // Calculate the total length of the string representation
    int total_length = 200; // Adjust this value according to your needs
    char* str = new char[total_length];

    // Use snprintf to format the string
    snprintf(str, total_length, "Vector2d: x_i=%d, y_i=%d, x=%d, y=%d, vel_x=%d, vel_y=%d, "
                                "acc_x=%d, acc_y=%d, angle=%d, t0_x=%u, t0_y=%u, "
                                "direction=%d, pivot=%d",
                x_i, y_i, x, y, vel_x, vel_y, acc_x, acc_y, angle, t0_x, t0_y,
                static_cast<int>(direction), static_cast<int>(pivot));

    return str;
}
