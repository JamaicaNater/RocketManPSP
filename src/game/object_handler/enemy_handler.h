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

    EnemyHandler(int MAX_OBJECTS, int _velocity,
        int _time_between_spawns, Object::ObjectTypes _type);
    ~EnemyHandler();

private:
    Image img;
};
