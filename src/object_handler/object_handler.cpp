#include "object_handler.h"
#include "../logger/logger.h"
#include "../Vector2d.hpp"
#include "../graphics/gfx.hpp"

ObjectHandler::ObjectHandler(int MAX_OBJECTS, int _velocity, 
    int _time_between_spawns, Object::ObjectTypes _type)
{
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Calling ObjectList for objects");
    object_list = *(new ObjectList(MAX_OBJECTS));

    velocity = _velocity;
    time_between_spawns = _time_between_spawns;
    type = _type;
}

ObjectHandler::~ObjectHandler(){
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Attempt destruction of Object handler "
        "with list of size %d", object_list.MAX_SIZE);

    object_list.~ObjectList();
    free(&object_list);
}

bool ObjectHandler::can_spawn(){
    return ( (curr_time >= last_spawn + time_between_spawns) && 
             object_list.size < object_list.MAX_SIZE );
}

void ObjectHandler::spawn(Vector2d v, Image _img) {
    if (!can_spawn()) return;

    Object * object = new Object(_img);
    object->vector = v;
    object->type = type;

    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Spawned:%s ptr: %0x, %d", 
    _img.filename, _img.img_matrix, _img.height);

    PSP_LOGGER::assert((object_list.insert(object) > -1), 
        "Object spawned successfully");
    last_spawn = curr_time;
}

void ObjectHandler::update_physics(){
    remove_dead_objects();

    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        objects[i]->vector.x += objects[i]->vector.vel_x;
        objects[i]->vector.y = noise_map[objects[i]->vector.x];
    }
}

void ObjectHandler::draw(){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;
        if (objects[i]->off_screen()) continue;
        
        GFX::drawBMP(objects[i]->get_draw_x(), objects[i]->get_draw_y(), 
            objects[i]->vector.get_angle(), objects[i]->vector.pivot, 
            objects[i]->vector.direction, 0, objects[i]->image);
    }
}

void ObjectHandler::remove_dead_objects(){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        if (objects[i]->health <= 0) object_list.remove(objects[i]);
    }

}

void ObjectHandler::clean(){
    PSP_LOGGER::log(PSP_LOGGER::WARNING, "Warning, pure virtual clean method "
        "called, this does nothing");
}

void ObjectHandler::init() {
    PSP_LOGGER::log(PSP_LOGGER::WARNING, "Warning, pure virtual init method "
        "called, this does nothing");
}
