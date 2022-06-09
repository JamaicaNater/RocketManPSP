#include "globals.h"
#include "math.h"
#include "physics.h"

#include "../logger/logger.h"

void apply_gravity(Vector2d &vec){
    
    float time = (int)(curr_time - vec.t0_y) / (float)SECOND;
    time*=2;
    vec.y= vec.y_i + vec.vel_y*(time) + .5 * Vector2d::grav/2 * pow(time, 2) ;
}