#include "animation_handler.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"
#include "../bmp/loadbmp.h"

AnimationHandler::AnimationHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns) : ObjectHandler(MAX_OBJECTS, _velocity, _time_between_spawns)
{
    frame_data = (FrameData*)malloc(MAX_OBJECTS * sizeof(FrameData));
    memset(frame_data, 0, sizeof(FrameData) * MAX_OBJECTS);
    time_between_spawns = 0;
}

AnimationHandler::~AnimationHandler()
{
    free(frame_data);
}

void AnimationHandler::update_frames(int game_time){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "%d + %d > %d", frame_data[i].last_updated , FRAME_TIME , game_time);
        if (frame_data[i].last_updated + FRAME_TIME < game_time){
            frame_data[i].curr_frame++;
            frame_data[i].last_updated = game_time;

            if (frame_data[i].curr_frame >= animation->cols*animation->rows){
                frame_data[i] = {0,0};
                object_list.remove(objects[i]);
            }
        }
    }
}

void AnimationHandler::draw(int cam_pos_x){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++) if (objects[i]) objects[i]->image = animation->get_frame(frame_data[i].curr_frame);
    ObjectHandler::draw(cam_pos_x);
}

void AnimationHandler::init(Animation * _animation){
    load_BMP(*_animation);
    animation = _animation;
}

int AnimationHandler::get_size(){
    return object_list.size;
}