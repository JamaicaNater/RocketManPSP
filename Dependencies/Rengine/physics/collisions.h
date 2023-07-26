#pragma once

#include <set>

/**
 * @brief Check if two objects collide
 *
 * @param obj - The object we are checking the global object list against
 * @param collisions list of objects detected to colliding with
 * @param ignore object types we ignore collisions with
 * @return Object* - Number of objects we collided with.
 */
int object_collision(Object * obj, ObjectList &collisions, std::set<Object::ObjectTypes> ignore);

/**
 * @brief Detects collisions with the terrain
 *
 * @param obj the object we are checking
 * @return true - collision detected
 * @return false - no collision
 */
bool terrain_collision(Object * obj, char * terrain);
