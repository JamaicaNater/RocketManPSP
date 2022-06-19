#include "animation_handler.h"
#include "graphics/gfx.hpp"
#include "bmp/loadbmp.h"

AnimationHandler::AnimationHandler(int MAX_OBJECTS, int _velocity,
    int _time_between_spawns, Object::ObjectTypes _type)
    : ObjectHandler(MAX_OBJECTS, _velocity, _time_between_spawns, _type
) {

}

AnimationHandler::~AnimationHandler()
{
    psp_free(frame_data);
    if(animation) animation->~Animation();
}

void AnimationHandler::update_frames(){
    Object ** objects = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!objects[i]) continue;

        if (frame_data[i].last_updated + FRAME_TIME < curr_time){
            frame_data[i].curr_frame++;
            frame_data[i].last_updated = curr_time;

            if (frame_data[i].curr_frame >= animation->cols*animation->rows){
                frame_data[i] = {0,0};
                object_list.remove(objects[i]);
                continue;
            }

            objects[i]->image = animation->get_frame(frame_data[i].curr_frame);
        }
    }
}

void AnimationHandler::init(Animation * _animation){
    frame_data = (FrameData*)psp_malloc(object_list.MAX_SIZE * sizeof(FrameData));
    for (int i = 0; i < object_list.MAX_SIZE; i++) frame_data[i] = {0,0};

    load_BMP(*_animation);
    animation = _animation;
}
