#pragma once

#include "../objects.hpp"

struct ObjectList
{
private:
    Object * * objects;

    int _insert(Object * value);
    int _remove(Object * value);
public:
    static ObjectList * _global_object_list;
    int MAX_SIZE;
    int size = 0;

    ObjectList();
    ObjectList(int _max_size);
    ObjectList(const ObjectList &other);
    ~ObjectList();
    int insert(Object * &value);
    int remove(Object* &value);
    Object * * get_list();
    bool is_full();
    bool is_empty();
};

