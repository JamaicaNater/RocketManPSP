#include "projectile_handler.h"

ProjectileHandler::ProjectileHandler(int MAX_OBJECTS, int velocity, int _time_between_spawns, AnimationHandler * _explosion_handler) : ObjectHandler(MAX_OBJECTS, velocity, _time_between_spawns)
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

        if (projectiles[i]->vector.y >= noise_map[projectiles[i]->vector.x]){ // Collision with floor
            // Object * explosion_object = new Object;
            // explosion_object->vector.x = projectiles[i]->vector.x;
            // explosion_object->vector.y = projectiles[i]->vector.y;
            // explosion_list.insert(explosion_object);
            // explosion_animation.animate = true;

            Image img = Image("assets/player.bmp"); //TODO: Eliminate this
            explosion_handler->spawn(Vector2d(projectiles[i]->vector.x, 
                projectiles[i]->vector.y), 2147483600, img);

            PSP_LOGGER::log(PSP_LOGGER::INFO, "exp size: %d", explosion_handler->get_size());
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
        //projectiles[i]->draw_pos_x = projectiles[i]->vector.x - cam_pos_x;
    }

    ProjectileHandler::clean();
}
