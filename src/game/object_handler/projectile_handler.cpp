#include "projectile_handler.h"

#include "physics/physics.h"
#include "physics/collisions.h"

ProjectileHandler::ProjectileHandler(int MAX_OBJECTS, int velocity, 
    int _time_between_spawns, Object::ObjectTypes type, 
    AnimationHandler * _explosion_handler) : 
    ObjectHandler(MAX_OBJECTS, velocity, _time_between_spawns, type
){
    explosion_handler = _explosion_handler;
    MAX_COLLISIONS = 4;
}

ProjectileHandler::~ProjectileHandler()
{
}

void ProjectileHandler::on_object_collision(Object * obj, ObjectList &collision_list){
    Object ** collisions = collision_list.get_list();
    for (int i = 0; i < collision_list.MAX_SIZE; i++){
        if (!collision_list.size) break;
        if (!collisions[i]) continue;

        collisions[i]->damage_object(37);
    }
    
    // Set collided object
    Object * coll_obj = collision_list.find_first();
    explosion_handler->spawn(
        Vector2d(coll_obj->vector.x, coll_obj->vector.y),
        explosion_handler->animation->get_frame(0));
        // If we collided with an object put the explosion under it 

    log(INFO, "Exploded proj at x:%d y:%d, coll_with:%d",
        obj->vector.x, obj->vector.y, collision_list.size);

    object_list.remove(obj);
}

void ProjectileHandler::on_terrain_collision(Object * obj){
    explosion_handler->spawn(
    Vector2d(obj->vector.x, obj->vector.y),
    explosion_handler->animation->get_frame(0));
    // Otherwise put the explosion when the projectile is. 

    log(INFO, "Exploded proj at x:%d y:%d, on terrain",
        obj->vector.x, obj->vector.y);

    object_list.remove(obj);
}

void ProjectileHandler::on_off_screen(Object * obj){
    log(INFO, "Freed proj. at x:%d y:%d",
        obj->vector.x, obj->vector.y);

    object_list.remove(obj);
}

void ProjectileHandler::update_physics(){
    Object ** projectiles = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!projectiles[i]) continue;

        float time = ((int)curr_time - (int)projectiles[i]->vector.t0_y) 
                    / 1000000.0f;
        
        apply_gravity(projectiles[i]->vector);

        projectiles[i]->vector.x= projectiles[i]->vector.x_i + 
            projectiles[i]->vector.vel_x * time;
    }

    ProjectileHandler::check_collisions(MAX_COLLISIONS);
}
