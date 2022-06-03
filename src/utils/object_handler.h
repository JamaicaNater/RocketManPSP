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
    Image img;
public:
    void spawn(int x, int y, int game_time);
    virtual void update_physics(int playerx);
    void draw(int cam_pos_x);
    ObjectHandler(int MAX_OBJECTS, int _velocity, Image _img);
    virtual ~ObjectHandler();
};
