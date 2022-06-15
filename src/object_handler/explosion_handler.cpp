#include "explosion_handler.h"

ExplosionHandler::ExplosionHandler(int MAX_OBJECTS, int _velocity, 
    int _time_between_spawns, Object::ObjectTypes _type) 
        : AnimationHandler(MAX_OBJECTS, _velocity,  _time_between_spawns, _type){

}
ExplosionHandler::~ExplosionHandler() {

}

void ExplosionHandler::on_object_collision(Object * obj, ObjectList &collision_list){
    Object ** collisions = collision_list.get_list();
    for (int i = 0; i < collision_list.MAX_SIZE; i++){
        if (!collision_list.size) break;
        if (!collisions[i]) continue;

        collisions[i]->damage_object(1);
    }
}