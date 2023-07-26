#include "object/object.h"


Object::Object() {}
Object::Object(Image _img) {
    image = _img;
}

void Object::damage_object(int damage) {
    if (curr_time < next_damage_event) return;

    health -= damage;
    next_damage_event = curr_time + invincibility_time;
}

bool Object::off_screen() {
    return get_draw_x() > SCREEN_WIDTH_RES + 50 || get_draw_x() < -50
    || get_draw_y() > SCREEN_HEIGHT + 50 || get_draw_y() < -50;
}

int Object::get_draw_x() {
    // Where to draw the player on the screen
    if (type == PLAYER && MAP_SIZE - vector.x > SCREEN_WIDTH/2) {
        if (vector.x > SCREEN_WIDTH/2) return SCREEN_WIDTH/2 - image.width/2;
    }

    return vector.x - camera_x - image.width/2;
}

int Object::get_draw_y() {
    return vector.y - image.height;
}
