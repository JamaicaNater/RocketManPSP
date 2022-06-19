#pragma once

#include "object/object.h"
#include "game/object_handler/global_object_manager.h"

struct ObjectList {
 public:
    int MAX_SIZE;
    int size = 0;

    ObjectList();
    explicit ObjectList(int _max_size);
    ObjectList(const ObjectList &other);
    ~ObjectList();

    /**
     * @brief Get object at a specific index
     *
     * @param index
     * @return Object* The object pointer
     */
    Object * get(int index);

    /**
     * @brief gets the first non null in the list
     *
     * @return Object*&  First non null in list, NULL if list is empty
     */
    Object * find_first();

    /**
     * @brief Searches for object in list
     *
     * @param obj object pointer we are searching for
     * @return int index of the object -1 if not found
     */
    int find(Object * obj);

    /**
     * @brief Insert into both global object list and the local list
     *
     * @param value - object we wish to insert
     * @return int - index if success -1 if not successful
     */
    int insert(Object * &value);

    /**
     * @brief Remove from both global object list and the local list
     *
     * @param value - object we wish to insert
     * @return int - index if success -1 if not found
     */
    int remove(Object* &value);

    /** //TODO: Abstract this away
     * @brief Get the list object
     *
     * @return Object**
     */
    Object * * get_list();

    /**
     * @brief
     *
     * @return true list is full
     * @return false list is not full
     */
    bool is_full();

    /**
     * @brief
     *
     * @return true list is empty
     * @return false list is not empty
     */
    bool is_empty();

 private:
    Object * * objects;
    static ObjectList * _global_object_list;
    friend int object_collision(Object*, ObjectList&);

    friend void ObjectManager::draw_health_bars();

    /**
     * @brief Private insert method, inserts to a given list without inserting
     * into the global object list
     *
     * @param value - object we wish to insert
     * @return int - index if success -1 if not successful
     */
    int _insert(Object * value);

    /**
     * @brief Private remove method, removes from a given list without removing
     * from the global object list
     *
     * @param value - object we wish to remove
     * @return int - index if success -1 if not found
     */
    int _remove(Object * value);
};
