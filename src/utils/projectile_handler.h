#pragma once

#include "object_handler.h"

class ProjectileHandler : public ObjectHandler
{
private:
    
public:
    ProjectileHandler(int MAX_OBJECTS, int _velocity);
    ~ProjectileHandler();

    void clean();
};