#pragma once

#include "object_list.hpp"
#include "../globals.h"
#include "../Vector2d.hpp"

class ObjectHandler
{
protected:
    ObjectList object_list;
    unsigned int time_between_spawns = 0;
    unsigned int last_spawn = 0;
    Object::ObjectTypes type = Object::NONE;
    int velocity;
public:
    void spawn(Vector2d v, int game_time, Image &_img);
    bool can_spawn(int game_time);
    virtual void update_physics(int game_time);
    virtual void draw(int cam_pos_x);
    virtual void clean();
    virtual void init();
    ObjectHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, Object::ObjectTypes type);
    virtual ~ObjectHandler();
};
