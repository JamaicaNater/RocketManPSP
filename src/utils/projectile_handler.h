#pragma once

#include "object_handler.h"
#include "animation_handler.h"

class ProjectileHandler : public ObjectHandler
{
private:
    AnimationHandler * explosion_handler;
    
public:
    ProjectileHandler(int MAX_OBJECTS, int velocity, int _time_between_spawns, 
    Object::ObjectTypes type, AnimationHandler * _explosion_handler);
    ~ProjectileHandler();

    void clean();
    void update_physics(int game_time);
};