#include "object_handler.h"
#include "../logger/logger.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"

ObjectHandler::ObjectHandler(int MAX_OBJECTS, int _velocity)
{
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Calling ObjectList for objects");
    object_list = *(new ObjectList(MAX_OBJECTS));
    Object ** objects = object_list.get_list();

    velocity = _velocity;
}

ObjectHandler::~ObjectHandler(){
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Attempt destruction of Object handler with list of size %d", object_list.MAX_SIZE);
    object_list.~ObjectList();
    free(&object_list);
}

void ObjectHandler::spawn(int x, int y, int game_time, Image _img) {
    if (game_time < last_spawn + time_between_spawns) return;
    if (object_list.size >= object_list.MAX_SIZE){
        //PSP_LOGGER::log(PSP_LOGGER::ERROR, "Spawn fail: Object list full");
        return;
    }

    Object * object = new Object(_img);
    object->vector.x = x;
    object->vector.y = y;

    PSP_LOGGER::assert((object_list.insert(object) > -1), 
        "Object spawned successfully");
    last_spawn = game_time;
}

void ObjectHandler::update_physics(int player_x){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        objects[i]->vector.x += objects[i]->vector.vel_x;
        objects[i]->vector.y = noise_map[objects[i]->vector.x];
    }
}

void ObjectHandler::draw(int cam_pos_x){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;
        
        if (!objects[i]->off_screen()) {
            GFX::drawBMP(objects[i]->vector.x - cam_pos_x, objects[i]->vector.y, 
            objects[i]->vector.get_angle(), CENTER, 
            objects[i]->vector.direction, 0, objects[i]->image);
        }

    }
}

void ObjectHandler::clean(){
    PSP_LOGGER::log(PSP_LOGGER::WARNING, "Warning, base class clean method "
    "called, this does nothing");
}
