#pragma once

#include "object_handler.h"

class ExplosionHandler : public ObjectHandler
{
private:
    struct FrameData
    {
        int last_updated = 0;
        int curr_frame = 0;
    };

    FrameData * frame_data = NULL;
    const int FRAME_TIME = 30 * MILLISECOND;
    Animation exp_animation = Animation(3, 5, 50000, "assets/explosion.bmp");
public:
    ExplosionHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns);
    ~ExplosionHandler();

    void init();
    void update_frames(int game_time);
    virtual void draw(int cam_pos_x);
    int get_size();
};
