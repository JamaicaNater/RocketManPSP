#include "../globals.h"

#include "../object_handler/object_list.h"
#include "../logger/logger.h"
#include "collisions.h"

Object * object_collision(Object * obj){
    // Treat objects as a rectangle
    // NOTE: a position of 0 is the higher than a position of 50
    // NOTE: objects are shifter left by half their width when drawn
    int obj1_x1 = obj->vector.x - obj->image.width/2, 
        obj1_y1 = obj->vector.y - obj->image.height, // Top Left
        obj1_x2 = obj->vector.x + obj->image.width/2, 
        obj1_y2 = obj->vector.y,//Bottom right
        obj2_x1, obj2_x2, obj2_y1, obj2_y2;

    Object ** glob_objects = ObjectList::_global_object_list->get_list();

    for (int i = 0; i < ObjectList::_global_object_list->MAX_SIZE; i++){
        if (!glob_objects[i]) continue;
        if (glob_objects[i] == obj) continue;
        if (glob_objects[i]->type == Object::EXPLOSION) continue;

        obj2_x1 = glob_objects[i]->vector.x - glob_objects[i]->image.width/2, 
        obj2_y1 = glob_objects[i]->vector.y - glob_objects[i]->image.height, // Top Left
        obj2_x2 = glob_objects[i]->vector.x + glob_objects[i]->image.width/2, 
        obj2_y2 = glob_objects[i]->vector.y; // Bottom Right

        //PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Obj1:%d (%d,%d),(%d,%d); Obj2:%d (%d,%d),(%d,%d);", obj->type,obj1_x1,obj1_y1,obj1_x2,obj1_y2, glob_objects[i]->type,obj2_x1,obj2_y1,obj2_x2,obj2_y2);

        // If the leftmost x postion of object one is greater than the rightmost position of object 2 OR
        // If the right most position of object 1 is less that the left most position of object 2
        // Then they cannot be overlapping;
        if(obj1_x1>obj2_x2||obj1_x2<obj2_x1) continue;

        // NOTE: a position of 0 is the higher than a position of 50
        // If the bottom of object two is above the top of object one OR
        // If the bottom of object one is above the top of object two
        // They cannot overlap
        if(obj1_y1>obj2_y2||obj2_y1>obj1_y2) continue;
        
        PSP_LOGGER::log(PSP_LOGGER::INFO, "Collision detected with %0x", glob_objects[i]);
        return glob_objects[i];
    }
    return NULL;
}

bool terrain_collision(Object * obj){
    return obj->vector.y >= noise_map[obj->vector.x];
}