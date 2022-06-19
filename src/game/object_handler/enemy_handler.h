#pragma once

#include "object_handler.h"

class EnemyHandler : public ObjectHandler
{
public:
    /**
     * @brief Handle actions of the AI
     * 
     * @param player_x position of the player
     */
    void update_movement(int player_x);
    
    EnemyHandler(int MAX_OBJECTS, int _velocity, 
        int _time_between_spawns, Object::ObjectTypes _type);
    ~EnemyHandler();

private:
    Image img;
};
