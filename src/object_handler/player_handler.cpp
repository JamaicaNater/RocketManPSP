#include <pspkernel.h>
#include "player_handler.h"
#include "../globals.h"
#include "../utils.h"
#include <math.h>
#include "../graphics/gfx.hpp"
#include "../physics/physics.h"

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

    player->type = Object::PLAYER;
    //object_list.insert(player);

    weapon.vector.pivot = CENTER_LEFT;
    //object_list.insert(weapon);
}

Object * PlayerHandler::player = new Object(Image("assets/player.bmp"));

Vector2d PlayerHandler::get_player_vec(){
    return player->vector;
}

void PlayerHandler::read_controls(){
    player->vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    camera_x = get_cam_position(player->vector.x, SCREEN_WIDTH/2);
    sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 

    // PLAYER MOVEMENT
    if(ctrlData.Buttons & PSP_CTRL_LEFT){
        player->vector.vel_x = -1*velocity;
        player->vector.direction = BACKWARD;
        weapon.vector.direction = BACKWARD;
        weapon.vector.x = player->vector.x-10;
    } 		
    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player->vector.vel_x = velocity;
        player->vector.direction = FORWARD;
        weapon.vector.direction = FORWARD;
        weapon.vector.x = player->vector.x+10;
    }

    // Weapon angle positioning
    if(ctrlData.Buttons & PSP_CTRL_UP){ 
        weapon.vector.set_angle(weapon.vector.get_angle()-2);
    }
    if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
        weapon.vector.set_angle(weapon.vector.get_angle()+2);
    }

    // Jump 
    if (!player->vector.vel_y) player->vector.y = (int)noise_map[player->vector.x];
    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        if (player->vector.vel_y == 0) {
            player->vector.vel_y= -50; // you cant double jump
            player->vector.y_i = player->vector.y;
            player->vector.t0_y = curr_time;
        }
    }


    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER && projectile_handler->can_spawn() ){
        Vector2d vec;

        vec.t0_y = vec.t0_x = curr_time;
        vec.x = vec.x_i = weapon.vector.x;
        vec.y = vec.y_i = weapon.vector.y;
        vec.set_angle(weapon.vector.get_angle());
        vec.direction = weapon.vector.direction;
        vec.pivot = weapon.vector.pivot;

        float rad = PI/180.0f * vec.get_angle();
        vec.vel_x = cos(rad) * 500;
        vec.vel_y = sin(rad) * 500;

        projectile_handler->spawn(vec, rocket);
    }
}

void PlayerHandler::update_physics() {
    if (player->vector.vel_y) { //JUMP Physics
        apply_gravity(player->vector);
        if (player->vector.y > noise_map[player->vector.x]) { // End of jump
            player->vector.vel_y = player->vector.y_i = 0;
        }
    }

    // Dont move outside the map
    if (    player->vector.x+player->vector.vel_x > 0 && 
            player->vector.x+player->vector.vel_x <= MAP_SIZE-50  ) {
        player->vector.x+=player->vector.vel_x;
    }

    weapon.vector.y = player->vector.y-25;
}

void PlayerHandler::draw(){
    GFX::drawBMP(weapon.get_draw_x(), weapon.get_draw_y(), 
    weapon.vector.get_angle(), weapon.vector.pivot, weapon.vector.direction, 
    0, weapon.image);

    GFX::drawBMP(player->get_draw_x(), player->get_draw_y() , 0, CENTER, 
        player->vector.direction, 0, player->image);
}