#pragma once

#include "../objects.hpp"

struct ObjectList
{
    Object * * objects;
    int MAX_SIZE;
    int size = 0;

    ObjectList(){}

    ObjectList(int _max_size) {
        MAX_SIZE = _max_size;
        PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Allocating %d Objects", MAX_SIZE);
        objects = (Object**)malloc(MAX_SIZE * sizeof(Object *));
        memset(objects, 0, MAX_SIZE * sizeof(Object *));
    }

    ObjectList(const ObjectList &other){
        MAX_SIZE = other.MAX_SIZE;
        PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "(copy constructor)Allocating %d Objects", MAX_SIZE);
        objects = (Object**)malloc(MAX_SIZE * sizeof(Object *));
        memset(objects, 0, MAX_SIZE * sizeof(Object *));
    }

    ~ObjectList() {
        PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Deallocating %d Objects", MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; i++){
             if (objects[i]) free(objects[i]);
        }
        free(objects);
    }

    int insert(Object * value){
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

    int remove(Object * value){
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

    Object * * get_list() {
        return objects;
    }
};
