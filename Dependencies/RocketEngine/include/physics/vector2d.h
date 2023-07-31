#pragma once

#include <stdint.h>

enum Flips : int8_t {FORWARD = 1, BACKWARD = -1};
enum Position : uint8_t {CENTER = 0, TOP_LEFT, CENTER_LEFT, BOTTOM_LEFT,
    TOP_CENTER, BOTTOM_CENTER, TOP_RIGHT, CENTER_RIGHT, BOTTOM_RIGHT};

struct Vector2d {
 public:
    constexpr static float grav = 70.0f;
    int16_t x_i = 0, y_i = 0, x = 0, y = 0, vel_x = 0, vel_y = 0, acc_x = 0,
        acc_y = 0, angle = 0;
    uint32_t t0_x = 0, t0_y = 0;  //t0 for velocity and gravity calculations
    Flips direction;
    Position pivot = CENTER;

    Vector2d();

    Vector2d(int16_t _x, int16_t _y);

    /**
     * @brief Get the mirrored angle
     * 
     * @return short 
     */
    int16_t get_mirrored_angle();

    char* to_string() const;
 private:
};
