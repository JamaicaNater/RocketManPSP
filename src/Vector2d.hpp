#pragma once

enum Flips{FORWARD = 1, BACKWARD = -1};
enum pivots{CENTER = 0, TOP_LEFT, CENTER_LEFT, BOTTOM_LEFT, TOP_CENTER, BOTTOM_CENTER, TOP_RIGHT, CENTER_RIGHT, BOTTOM_RIGHT};
struct Vector2d {
    private:
        short angle = 0;
    public:
    constexpr static float grav = 70.0f;
    short x_i=0, y_i=0, x=0, y=0, vel_x=0, vel_y=0, acc_x=0, acc_y=0;
    unsigned int t0_x = 0, t0_y = 0; // t0 for velocity and gravit calculations
    Flips direction;
    pivots pivot = CENTER;

    Vector2d() {
        x = y = angle = 0;
    }

    Vector2d(short _x, short _y){
        x = _x;
        y = _y;
        angle = 0;
    }

    void set_angle(short _angle) {
        if (_angle > 360) _angle%=360;
        if (_angle < 360) _angle%=-360;
        angle = _angle;
    }

    short get_angle() {
        return angle;
    }
};