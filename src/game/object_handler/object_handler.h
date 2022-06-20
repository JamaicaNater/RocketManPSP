#pragma once

#include <set>

#include "object/object_list.h"
#include "physics/vector2d.h"

class ObjectHandler
{
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
    virtual void on_zero_health(Object * obj);

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
    virtual void draw(); // TODO remove virtual

    /**
     * @brief removes dead objects, can be overridden
     *
     */
    void clean();

    /**
     * @brief N/A pure virtual function
     *
     */
    virtual void init();

    ObjectHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns,
        Object::ObjectTypes type);
    virtual ~ObjectHandler();
protected:
    Object::ObjectTypes type = Object::NONE;
    ObjectList object_list;
    unsigned int time_between_spawns = 0;
    unsigned int last_spawn = 0;
    int velocity;
    int MAX_COLLISIONS = 0;
    std::set<Object::ObjectTypes> ignore_collisions_with;

    /**
     * @brief Checks to see if a given object has collided with anything
     * in the space, if so it will perform differnt event depending on
     * if the object collides with the floor another object or goes
     * off screen
     *
     * @param MAX_COLLISIONS
     */
    void check_collisions(int MAX_COLLISIONS);

    /**
     * @brief Pure virtual function decribes what do so if an object collides
     * with another
     *
     * @param obj object we check against all the others
     * @param collision_list
     */
    virtual void on_object_collision(Object * obj, ObjectList &collision_list);

    /**
     * @brief Pure virtual function decribes what do so if an object collides
     * with the terrain
     *
     * @param obj object we check
     */
    virtual void on_terrain_collision(Object * obj);

    /**
     * @brief Pure virtual function decribes what do so if an object goes
     * off screen
     *
     * @param obj object we check
     */
    virtual void on_off_screen(Object * obj);
};
