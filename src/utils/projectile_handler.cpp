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
        if (terrain_collision(projectiles[i]) || object_collision(projectiles[i])){ // Collision with floor
            Image img = explosion_handler->animation->get_frame(0); //TODO: Eliminate this
            explosion_handler->spawn(Vector2d(projectiles[i]->vector.x, 
                projectiles[i]->vector.y), 0, img); // NOTE: game time does not matter for explosion handler

            PSP_LOGGER::log(PSP_LOGGER::INFO, "Exploded projectile %d at scr_x:%d, x%d y:%d",i, projectiles[i]->draw_pos_x, projectiles[i]->vector.x, projectiles[i]->vector.y);
            object_list.remove(projectiles[i]);
        } else if (projectiles[i]->off_screen()) {
            PSP_LOGGER::log(PSP_LOGGER::INFO, "Freed projectile %d at scr_x:%d, x:%d y:%d",i, projectiles[i]->draw_pos_x, projectiles[i]->vector.x, projectiles[i]->vector.y);
            object_list.remove(projectiles[i]);
        }
    }
}

void ProjectileHandler::update_physics(int game_time){
    Object ** projectiles = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!projectiles[i]) continue;

        float time = ((int)game_time - (int)projectiles[i]->vector.created_at) / 1000000.0f;
        projectiles[i]->vector.y= projectiles[i]->vector.y_i + projectiles[i]->vector.vel_y*(time) + .5 * (Vector2d::grav * (time) * (time) );	
        projectiles[i]->vector.x= projectiles[i]->vector.x_i + projectiles[i]->vector.vel_x * time;
    }

    ProjectileHandler::clean();
}
