#pragma once

#include "object_handler.h"
#include "animation_handler.h"

class ProjectileHandler : public ObjectHandler
{
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
    
private:
    AnimationHandler * explosion_handler; // Trigger explosions
    
    /**
     * @brief When projectiles go off the screen delete them
     * 
     * @param obj projectile
     */
    void on_off_screen(Object * obj);

    /**
     * @brief When projectiles collide with objects, explode them, damage the 
     * objects the projectile collides with and then delete the projectile
     * 
     * @param obj projectile
     * @param collision_list objects collided with
     */
    void on_object_collision(Object * obj, ObjectList &collision_list);

    /**
     * @brief When the projectile collides with the terrain explode it, and
     * delete the projectile
     * 
     * @param obj 
     */
    void on_terrain_collision(Object * obj);
};