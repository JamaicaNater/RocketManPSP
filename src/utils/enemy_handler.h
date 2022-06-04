#pragma once

#include "object_handler.h"

class EnemyHandler : public ObjectHandler
{
private:
    Image img;
    /* data */
public:
    void update_physics();
    void update_movement(int player_x);
    EnemyHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns);
    ~EnemyHandler();
};
