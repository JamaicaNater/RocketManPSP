#pragma once

#include "object/object_handler.h"

class EnemyHandler : public ObjectHandler
{
public:
    /**
     * @brief Handle actions of the AI
     *
     * @param player_x position of the player
     */
    void update_movement(int player_x);

    /**
     * @brief Damage the player
     *
     * @param obj current enemy
     * @param collision_list list of objects
     */
    void on_object_collision(Object * obj, ObjectList &collision_list) override;

    /**
     * @brief increment enemies killed and then call base class on_zero_health
     * function
     *
     * @param obj
     */
    void on_zero_health(Object * obj) override;

    /**
     * @brief Create the handler for the Enemy object
     *
     * @param MAX_OBJECTS max objects
     * @param velocity speed of objects
     * @param time_between_spawns time for each object to spawn
     * @param position of the camera
     * @param terrain 
     * @param type type of object
     */
    EnemyHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns,
        int * _camera_x, unsigned char * _terrain, Object::ObjectTypes type);
    ~EnemyHandler();

private:
    Image img;
};
