#pragma once

#include "../objects.hpp"

struct ObjectList
{
private:
    Object * * objects;
    friend int object_collision(Object*, ObjectList&);

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
public:
    static ObjectList * _global_object_list;
    int MAX_SIZE;
    int size = 0;

    ObjectList();
    ObjectList(int _max_size);
    ObjectList(const ObjectList &other);
    ~ObjectList();

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
};

