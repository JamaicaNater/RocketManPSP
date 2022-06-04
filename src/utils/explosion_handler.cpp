#include "explosion_handler.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"
#include "../bmp/loadbmp.h"

ExplosionHandler::ExplosionHandler(int MAX_OBJECTS, int _velocity, int _time_between_spawns) : ObjectHandler(MAX_OBJECTS, _velocity, _time_between_spawns)
{
    frame_data = (FrameData*)malloc(MAX_OBJECTS * sizeof(FrameData));
    memset(frame_data, 0, sizeof(FrameData) * MAX_OBJECTS);
    time_between_spawns = 0;
}

ExplosionHandler::~ExplosionHandler()
{
    free(frame_data);
}

void ExplosionHandler::update_frames(int game_time){
    Object ** explosions = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!explosions[i]) continue;

        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "%d + %d > %d", frame_data[i].last_updated , FRAME_TIME , game_time);
        if (frame_data[i].last_updated + FRAME_TIME < game_time){
            frame_data[i].curr_frame++;
            frame_data[i].last_updated = game_time;

            if (frame_data[i].curr_frame >= exp_animation.cols*exp_animation.rows){
                frame_data[i] = {0,0};
                object_list.remove(explosions[i]);
            }
        }
    }
}

void ExplosionHandler::draw(int cam_pos_x){
    Object ** explosions = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++) if (explosions[i]) explosions[i]->image = exp_animation.get_frame(frame_data[i].curr_frame);
    ObjectHandler::draw(cam_pos_x);
}

void ExplosionHandler::init(){
    load_BMP(exp_animation);
}

int ExplosionHandler::get_size(){
    return object_list.size;
}