#include "enemy_handler.h"

#include "gamestate.h"

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
        if (collisions[i]->type != Object::PLAYER) continue;

        collisions[i]->damage_object(1);
    }
}

void EnemyHandler::on_zero_health(Object * obj) {
    GameState::enemies_killed++;
    ObjectHandler::on_zero_health(obj);
}

EnemyHandler::EnemyHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns,
    int * _camera_x, unsigned char * _terrain, Object::ObjectTypes _type) :
    ObjectHandler(MAX_OBJECTS, _velocity, _time_between_spawns, _camera_x, _terrain, _type 
) {
    MAX_COLLISIONS = 1;
    ignore_collisions_with = {Object::ENEMY, Object::WEAPON};
}

EnemyHandler::~EnemyHandler()
{
}