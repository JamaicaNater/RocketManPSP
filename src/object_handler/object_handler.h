#pragma once

#include "object_list.h"
#include "../globals.h"
#include "../Vector2d.hpp"

class ObjectHandler
{
protected:
    Object::ObjectTypes type = Object::NONE; 
    ObjectList object_list;
    unsigned int time_between_spawns = 0;
    unsigned int last_spawn = 0;
    int velocity;
public:
    /**
     * @brief Spawn an Object at a point specifed by vector
     * 
     * @param v - vector to assign to the object
     * @param _img // TODO eliminate this parameter
     */
    void spawn(Vector2d v, Image _img);

    /**
     * @brief Determine if we can spawn the object, check to se if our list is 
     * full or if enough time has passed for the spawn to happen
     * 
     * @return true 
     * @return false 
     */
    bool can_spawn();

    /**
     * @brief Remove objects that have under 0 heath
     * 
     */
    void remove_dead_objects();

    /**
     * @brief Advance the postion of the object;
     * 
     * TODO: disconnect gamespeed from player speed
     */
    virtual void update_physics();

    /**
     * @brief Draw objects on the screen
     * 
     */
    void draw();

    /**
     * @brief N/A pure virtual function
     * 
     */
    virtual void clean();

    /**
     * @brief N/A pure virtual function
     * 
     */
    virtual void init();
    ObjectHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns, 
        Object::ObjectTypes type);
    virtual ~ObjectHandler();
};
