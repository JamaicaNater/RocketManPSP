#include "globals.h"
#include "math.h"
#include "physics.h"

#include "../logger/logger.h"

void apply_gravity(Vector2d &vec){
    float time = ((int)curr_time - (int)vec.t0_y) / 1000000.0f;
    time*=2;
    vec.y= vec.y_i + vec.vel_y*(time) + .5 * (Vector2d::grav * pow(time, 2) );
}