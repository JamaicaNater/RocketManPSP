#pragma once

#include "object_handler.h"

class EnemyHandler : public ObjectHandler
{
private:
    /* data */
public:
    void update_physics();
    void update_movement(int player_x);
    EnemyHandler(int MAX_OBJECTS, int _velocity, Image _img);
    ~EnemyHandler();
};
