#include "object_handler/animation_handler.h"

class ExplosionHandler : public AnimationHandler {
public:
    ExplosionHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns,
    int * camera_x, unsigned char * terrain, Object::ObjectTypes _type);

    ~ExplosionHandler();
    
    /**
     * @brief Damage all objects that the explosion collides with
     * 
     * @param obj 
     * @param collision_list 
     */
    virtual void on_object_collision(Object * obj, ObjectList &collision_list) override;
private:
    
};
