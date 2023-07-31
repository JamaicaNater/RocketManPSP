#pragma once

#include "object/object_handler.h"
#include "image/animation.h"

class AnimationHandler : public ObjectHandler
{
public:
    AnimationHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, 
        int * camera_x, unsigned char * terrain, Object::ObjectTypes _type);
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
private:
    friend class ProjectileHandler;
    struct FrameData
    {
        uint32_t last_updated = 0;
        uint32_t curr_frame = 0;
    };

    FrameData * frame_data = NULL;
    Animation * animation = NULL;

    const uint32_t FRAME_TIME = 30 * MILLISECOND;
};
