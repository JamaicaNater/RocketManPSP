#pragma once

#include "object_handler.h"

class AnimationHandler : public ObjectHandler
{
private:
    friend class ProjectileHandler;
    struct FrameData
    {
        unsigned int last_updated = 0;
        unsigned int curr_frame = 0;
    };

    FrameData * frame_data = NULL;
    Animation * animation = NULL;

    const unsigned int FRAME_TIME = 30 * MILLISECOND;
    void on_object_collision(Object * obj, ObjectList &collision_list);
public:
    AnimationHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, 
        Object::ObjectTypes _type);
    ~AnimationHandler();

    /**
     * @brief Load the Animation into memory
     * 
     * @param _animation 
     */
    void init(Animation * _animation);

    /**
     * @brief update what frame of the animation the image of the object is on
     * 
     */
    void update_frames();
};
