#pragma once

#include <cstring>
#include <cstdlib>

#include "physics/vector2d.h"
#include "logger/logger.h"
#include "image/image.h"
#include "globals.h"

class Object {
 public:
    enum ObjectTypes{PLAYER = 0, ENEMY, MISSILE, EXPLOSION, WEAPON, NONE};

    ObjectTypes type = NONE;
    Image image;
    int draw_pos_x; // postion to draw at
    int health = 100, max_health = 100;

    uint32_t next_damage_event = 0;
    uint32_t invincibility_time = 10 * MILLISECOND;

    Vector2d vector;

    Object();

    explicit Object(Image _img);

    /**
     * @brief Reduce the health value of an object
     *
     * @param damage value to reduce objects health by
     */
    void damage_object(int damage);

    /**
     * @brief Returns whether or not if the object is off the screen
     *
     * @return true
     * @return false
     */
    bool off_screen();

    /**
     * @brief We dont draw objects as their precise x and y position, the x and
     * y refer to the position of the image corner
     *
     * @return int
     */
    int get_draw_x();

    /**
     * @brief We dont draw objects as their precise x and y position, the x and
     * y refer to the position of the image corner
     *
     * @return int
     */
    int get_draw_y();
};
