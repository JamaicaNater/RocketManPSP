#pragma once

#include "object_list.hpp"
#include "../globals.h"

class ObjectHandler
{
protected:
    ObjectList object_list;
    unsigned int time_between_spawns = 1000*1000;
    unsigned int last_spawn = 0;

    int velocity;
public:
    void spawn(int x, int y, int game_time, Image _img);
    virtual void update_physics(int playerx);
    virtual void draw(int cam_pos_x);
    virtual void clean();
    ObjectHandler(int MAX_OBJECTS, int _velocity);
    virtual ~ObjectHandler();
};
