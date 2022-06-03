#include "projectile_handler.h"

ProjectileHandler::ProjectileHandler(int MAX_OBJECTS, int velocity) : ObjectHandler(MAX_OBJECTS, velocity)
{
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

            PSP_LOGGER::log(PSP_LOGGER::INFO, "Exploded projectile %d located at scr_x%d, y%d",i, projectiles[i]->draw_pos_x, projectiles[i]->vector.y);
            object_list.remove(projectiles[i]);
        } else if (projectiles[i]->off_screen()) {
            PSP_LOGGER::log(PSP_LOGGER::INFO, "Freed projectile %d located at scr_x%d, y%d",i, projectiles[i]->draw_pos_x, projectiles[i]->vector.y);
            object_list.remove(projectiles[i]);
        }
    }
}
