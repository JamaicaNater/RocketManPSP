#include <pspkernel.h>
#include "player_handler.h"
#include "../globals.h"
#include "../utils.h"
#include <math.h>
#include "../graphics/gfx.hpp"
#include "../physics/physics.h"
#include "../bmp/loadbmp.h"

PlayerHandler::PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler) : 
    ObjectHandler(2, _velocity, 0, Object::PLAYER
) {
    projectile_handler = _projectile_handler;
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::init() {
    // For Controls
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    load_BMP(rocket);

    player->type = Object::PLAYER;
    object_list.insert(player);

    weapon->vector.pivot = CENTER_LEFT;
    weapon->type = Object::WEAPON; // TODO Weapon break mechanic?
    object_list.insert(weapon);
    player->health = 1000;
    player->max_health = 1000;

    projectile_handler->init();
}

Object * PlayerHandler::player = new Object(Image("assets/player.bmp"));

Object PlayerHandler::get_player_val(){
    return *player;
}

void PlayerHandler::read_controls(){
    player->vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    camera_x = get_cam_position(player->vector.x, SCREEN_WIDTH/2);
    sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 

    // PLAYER MOVEMENT
    if(ctrlData.Buttons & PSP_CTRL_LEFT){
        player->vector.vel_x = -1*velocity;
        player->vector.direction = BACKWARD;
        weapon->vector.direction = BACKWARD;
        weapon->vector.x = player->vector.x-10;
    } 		
    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player->vector.vel_x = velocity;
        player->vector.direction = FORWARD;
        weapon->vector.direction = FORWARD;
        weapon->vector.x = player->vector.x+10;
    }

    // Weapon angle positioning
    if(ctrlData.Buttons & PSP_CTRL_UP){ 
        weapon->vector.set_angle(weapon->vector.get_angle()-2);
    }
    if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
        weapon->vector.set_angle(weapon->vector.get_angle()+2);
    }

    // Jump 
    if (!player->vector.vel_y) player->vector.y = (int)noise_map[player->vector.x];
    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        if (player->vector.vel_y == 0) {
            player->vector.vel_y= -40; // you cant double jump
            player->vector.y_i = player->vector.y;
            player->vector.t0_y = curr_time;
        }
    }


    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER && projectile_handler->can_spawn() ){
        Vector2d vec;

        vec.t0_y = vec.t0_x = curr_time;
        vec.set_angle(weapon->vector.get_angle());
        vec.direction = weapon->vector.direction;
        vec.pivot = weapon->vector.pivot;

        float rad = PI/180.0f * ((player->vector.direction==FORWARD)*vec.get_angle() 
            + (player->vector.direction==BACKWARD)*vec.get_mirrored_angle());

        vec.x = vec.x_i = weapon->vector.x + cos(rad)*25;
        vec.y = vec.y_i = weapon->vector.y + sin(rad)*25;
        vec.vel_x = cos(rad) * 550;
        vec.vel_y = sin(rad) * 550;

        PSP_LOGGER::log(PSP_LOGGER::DEBUG, "ang: %d, mirr: %d", vec.get_angle(), vec.get_mirrored_angle());
        projectile_handler->spawn(vec, rocket);
    }
}

void PlayerHandler::update_physics() {
    ObjectHandler::update_physics();

    weapon->vector.y = player->vector.y-25;
}

void PlayerHandler::on_off_screen(Object * obj)  {
    if (player->vector.x < 0) player->vector.x = 20;
    if (player->vector.x > MAP_SIZE) player->vector.x = MAP_SIZE - 50;
}

