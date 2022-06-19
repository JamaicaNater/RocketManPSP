#include "enemy_handler.h"

void EnemyHandler::update_movement(int player_x){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        if (player_x < objects[i]->vector.x) {
            objects[i]->vector.vel_x = -1 * velocity;
            objects[i]->vector.direction = FORWARD;
        } else {
            objects[i]->vector.vel_x = 1 * velocity;
            objects[i]->vector.direction = BACKWARD;
        }
    }
}

void EnemyHandler::on_object_collision(Object * obj,
    ObjectList &collision_list
) {
    log (DEBUG, "called");
    Object ** collisions = collision_list.get_list();
    for (int i = 0; i < collision_list.MAX_SIZE; i++){
        if (!collision_list.size) break;
        if (!collisions[i]) continue;
        //if (collisions[i]->type != Object::PLAYER) continue;

        collisions[i]->damage_object(1);
    }
}

EnemyHandler::EnemyHandler(int MAX_OBJECTS, int _velocity,
    int _time_between_spawns, Object::ObjectTypes _type) :
    ObjectHandler(MAX_OBJECTS, _velocity, _time_between_spawns, _type
) {
    MAX_COLLISIONS = 1;
    ignore_collisions_with = {Object::ENEMY};
}

EnemyHandler::~EnemyHandler()
{
}