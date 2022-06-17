#include "object_list.h"
#include "../utils/psp_malloc.h"

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
    //log(DEBUG, "Allocating %d Objects", MAX_SIZE);
    objects = (Object**)psp_malloc(MAX_SIZE * sizeof(Object *));
    memset(objects, 0, MAX_SIZE * sizeof(Object *));
}

ObjectList::ObjectList(const ObjectList &other){
    MAX_SIZE = other.MAX_SIZE;
    //log(DEBUG, "(copy constructor) Allocating %d "
    //    "Objects", MAX_SIZE);

    objects = (Object**)psp_malloc(MAX_SIZE * sizeof(Object *));
    memset(objects, 0, MAX_SIZE * sizeof(Object *));
}

ObjectList::~ObjectList() {
    //log(DEBUG, "Deallocating %d Objects", MAX_SIZE);
    //TODO: list that refrence other list vs orignal list

    // for (int i = 0; i < MAX_SIZE; i++){
    //      if (objects[i]) free(objects[i]);
    // }
    psp_free(objects);
}

Object * ObjectList::get(int index) {
    return objects[index];
}

Object * ObjectList::find_first(){
    int i = MAX_SIZE;
    while (i > -1) {
        i--;
        if(objects[i]) return objects[i];
    }
    return NULL;
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

int ObjectList::find(Object * obj){
    int i = MAX_SIZE;
    while (i > -1) {
        i--;
        if(objects[i] == obj) {
            break;
        }
    }
    return i;
}