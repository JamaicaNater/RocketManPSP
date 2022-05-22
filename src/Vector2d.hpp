struct Vector2d {
    private:
        short angle;
    public:
    unsigned short x, y;
    bool direction;

    Vector2d() {
        x = y = angle = 0;
    }

    Vector2d(unsigned short _x, unsigned short _y){
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