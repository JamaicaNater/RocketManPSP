#include "explosion_handler.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"

ExplosionHandler::ExplosionHandler(int MAX_OBJECTS, int _velocity) : ObjectHandler(MAX_OBJECTS, _velocity)
{
    curr_frame = (int*)malloc(MAX_OBJECTS * sizeof(int));
}

ExplosionHandler::~ExplosionHandler()
{
    free(curr_frame);
}

void ExplosionHandler::draw(int cam_pos_x){
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