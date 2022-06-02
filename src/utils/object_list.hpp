#include "../objects.hpp"

struct ObjectList
{
    Object * * objects;
    int MAX_SIZE;
    int size = 0;

    ObjectList(int _max_size) {
        MAX_SIZE = _max_size;
        objects = (Object**)malloc(MAX_SIZE * sizeof(Object *));
        memset(objects, 0, MAX_SIZE * sizeof(Object *));
    }

    ~ObjectList() {
        for (int i = 0; i < size; i++){
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
