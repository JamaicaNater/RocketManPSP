#pragma once

#include "object_handler.h"

class AnimationHandler : public ObjectHandler
{
private:
    struct FrameData
    {
        int last_updated = 0;
        int curr_frame = 0;
    };

    FrameData * frame_data = NULL;
    Animation * animation;

    const int FRAME_TIME = 30 * MILLISECOND;
public:
    AnimationHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns);
    ~AnimationHandler();

    void init(Animation * _animation);
    void update_frames(int game_time);
    virtual void draw(int cam_pos_x);
};
