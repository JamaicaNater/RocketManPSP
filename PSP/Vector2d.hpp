struct Vector2d {
    unsigned short x, y, angle;
    bool direction;

    Vector2d() {

    }

    Vector2d(unsigned short _x, unsigned short _y){
        x = _x;
        y = _y;
    }
};