#include "animation_handler.h"
class ExplosionHandler : public AnimationHandler
{
public:
    ExplosionHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, Object::ObjectTypes _type);
    ~ExplosionHandler();

    virtual void on_object_collision(Object * obj, ObjectList &collision_list) override;
private:
    
};
