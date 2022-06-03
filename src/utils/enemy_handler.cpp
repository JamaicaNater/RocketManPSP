#include "enemy_handler.h"

void EnemyHandler::update_physics(){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        objects[i]->vector.x += objects[i]->vector.vel_x;
        objects[i]->vector.y = noise_map[objects[i]->vector.x];
    }
}

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

EnemyHandler::EnemyHandler(int MAX_OBJECTS, int _velocity, Image _img) : ObjectHandler(MAX_OBJECTS, _velocity, _img)
{
}

EnemyHandler::~EnemyHandler()
{
}