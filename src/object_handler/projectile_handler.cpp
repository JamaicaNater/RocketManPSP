#include "projectile_handler.h"
#include "../physics/collisions.h"

ProjectileHandler::ProjectileHandler(int MAX_OBJECTS, int velocity, int _time_between_spawns, Object::ObjectTypes type, AnimationHandler * _explosion_handler) : ObjectHandler(MAX_OBJECTS, velocity, _time_between_spawns, type)
{
    explosion_handler = _explosion_handler;
}

ProjectileHandler::~ProjectileHandler()
{
}

void ProjectileHandler::clean(){
    Object ** projectiles = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!projectiles[i]) continue;
        
        Object* coll_obj;
        if ( (coll_obj = object_collision(projectiles[i])) || 
            terrain_collision(projectiles[i]))
        { // Collision with floor
            
            if (coll_obj) coll_obj->health-=5;

            explosion_handler->spawn(
                Vector2d(
                (coll_obj) ? coll_obj->vector.x : projectiles[i]->vector.x,
                (coll_obj) ? coll_obj->vector.y : projectiles[i]->vector.y ),
                explosion_handler->animation->get_frame(0));
                // If we collided with an object put the explosion under it 
                // Otherwise put the explosion when the projectile is.

            PSP_LOGGER::log(PSP_LOGGER::INFO, "Exploded proj. %d, x%d y:%d",
                i, projectiles[i]->vector.x, projectiles[i]->vector.y);

            object_list.remove(projectiles[i]);
        } else if (projectiles[i]->off_screen()) {
            PSP_LOGGER::log(PSP_LOGGER::INFO, "Freed proj. %d, x:%d y:%d",i,
                 projectiles[i]->vector.x, projectiles[i]->vector.y);
                 
            object_list.remove(projectiles[i]);
        }
    }
}

void ProjectileHandler::update_physics(){
    Object ** projectiles = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!projectiles[i]) continue;

        float time = ((int)curr_time - (int)projectiles[i]->vector.created_at) 
                    / 1000000.0f;

        projectiles[i]->vector.y= projectiles[i]->vector.y_i + 
            projectiles[i]->vector.vel_y*(time) + .5 * 
            (Vector2d::grav * (time) * (time) );

        projectiles[i]->vector.x= projectiles[i]->vector.x_i + 
            projectiles[i]->vector.vel_x * time;
    }

    ProjectileHandler::clean();
}
