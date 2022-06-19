#include <math.h>

#include "globals.h"

#include "physics/physics.h"

#include "logger/logger.h"

void apply_gravity(Vector2d &vec) {
    float time = (int)(curr_time - vec.t0_y) / (float)SECOND;
    time*=2.5f;
    vec.y = vec.y_i + vec.vel_y*(time) 
        + .5f * Vector2d::grav/2.0f * pow(time, 2.0f);
}
