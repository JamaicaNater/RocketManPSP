#include "object_list.h"

int ObjectList::_insert(Object * value){
    int i = MAX_SIZE;

    while (i > -1)
    {
        i--;
        if(!objects[i]) {
            objects[i] = value;
            break;
        }
    }
    if (i != -1) size++;
    return i;
}

int ObjectList::_remove(Object * value){
    int i = MAX_SIZE;

    while (i > -1)
    {
        i--;
        if(objects[i] == value) {
            objects[i] = NULL;
            break;
        }
    }
    if (i != -1) size--;
    return i;
}

ObjectList * ObjectList::_global_object_list = new ObjectList(256);

ObjectList::ObjectList(){}

ObjectList::ObjectList(int _max_size) {
        MAX_SIZE = _max_size;
        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Allocating %d Objects", MAX_SIZE);
        objects = (Object**)malloc(MAX_SIZE * sizeof(Object *));
        memset(objects, 0, MAX_SIZE * sizeof(Object *));
    }

ObjectList::ObjectList(const ObjectList &other){
        MAX_SIZE = other.MAX_SIZE;
        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "(copy constructor)Allocating %d "
            "Objects", MAX_SIZE);
            
        objects = (Object**)malloc(MAX_SIZE * sizeof(Object *));
        memset(objects, 0, MAX_SIZE * sizeof(Object *));
    }

ObjectList::~ObjectList() {
        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Deallocating %d Objects", MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; i++){
             if (objects[i]) free(objects[i]);
        }
        free(objects);
    }

int ObjectList::insert(Object * &value){
        _global_object_list->_insert(value);
        return _insert(value);
    }

int ObjectList::remove(Object* &value){
        _global_object_list->_remove(value);
        return _remove(value);
    }

Object * * ObjectList::get_list() {
        return objects;
    }

bool ObjectList::is_full(){
        return size == MAX_SIZE;
    }

bool ObjectList::is_empty(){
        return size == 0;
    }