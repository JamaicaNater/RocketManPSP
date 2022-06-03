#include "object_handler.h"

class ExplosionHandler : public ObjectHandler
{
private:
    int * curr_frame;
    /* data */
public:
    ExplosionHandler(int MAX_OBJECTS, int _velocity);
    ~ExplosionHandler();

    virtual void draw(int cam_pos_x);
};
