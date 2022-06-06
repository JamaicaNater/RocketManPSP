#pragma once

#include "object_handler.h"
#include "animation_handler.h"

class ProjectileHandler : public ObjectHandler
{
private:
    AnimationHandler * explosion_handler; // Trigger explosions
    
public:
    ProjectileHandler(int MAX_OBJECTS, int velocity, int _time_between_spawns, 
    Object::ObjectTypes type, AnimationHandler * _explosion_handler);
    ~ProjectileHandler();

    /**
     * @brief Explode projectiles that collide and remove them from the list of 
     * projectiles. Remove off screen projectiles.
     * 
     */
    void clean();

    /**
     * @brief Advance movement of projectile
     * 
     * @param game_time 
     */
    void update_physics();
};