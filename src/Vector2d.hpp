struct Vector2d {
    private:
        short angle;
    public:
    constexpr static float grav = 60.0f;
    short x_i=0, y_i=0, x=0, y=0, vel_x=0, vel_y=0, acc_x=0, acc_y=0;
    unsigned int created_at;
    bool direction;

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