#pragma once

#include "object_handler.h"
#include "explosion_handler.h"

class ProjectileHandler : public ObjectHandler
{
private:
    ExplosionHandler * explosion_handler;
    
public:
    ProjectileHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, ExplosionHandler * explosion_handler);
    ~ProjectileHandler();

    void clean();
    void update_physics(int game_time);
};