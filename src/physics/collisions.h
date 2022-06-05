/**
 * @brief Check if two objects collide
 * 
 * @param obj - The object we are checking the global object list against
 * @return Object* - The first object we find that it collides with.
 *                 - NULL if no such object.
 */
Object * object_collision(Object * obj);

/**
 * @brief Detects collisions with the terrain
 * 
 * @param obj the object we are checking
 * @return true - collision detected
 * @return false - no collision
 */
bool terrain_collision(Object * obj);