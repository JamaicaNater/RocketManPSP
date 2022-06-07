#include "player_handler.h"
#include "../globals.h"
#include "../utils.h"
#include <math.h>
#include "../graphics/gfx.hpp"

PlayerHandler::PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler) : 
    ObjectHandler(1, _velocity, 0, Object::PLAYER
) {
    projectile_handler = _projectile_handler;
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::init() {
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void PlayerHandler::read_controls(){
    player.vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    camera_x = get_cam_position(player.vector.x, SCREEN_WIDTH/2);
    sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 

    // PLAYER MOVEMENT
    if(ctrlData.Buttons & PSP_CTRL_LEFT){
        player.vector.vel_x = -1*velocity;
        player.vector.direction = BACKWARD;
        player.weapon.vector.x = player.vector.x-10;
    } 		
    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player.vector.vel_x = velocity;
        player.vector.direction = FORWARD;
        player.weapon.vector.x = player.vector.x+10;
    }

    // Weapon angle positioning
    if(ctrlData.Buttons & PSP_CTRL_UP){ 
        player.weapon.vector.set_angle(player.weapon.vector.get_angle()-2);
    }
    if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
        player.weapon.vector.set_angle(player.weapon.vector.get_angle()+2);
    }

    // Jump 
    if (!player.vector.vel_y) player.vector.y = (int)noise_map[player.vector.x];
    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        if (player.vector.vel_y == 0) {
            player.vector.vel_y= 50; // you cant double jump
            player.jump_time = curr_time;
            player.starting_jump_height = player.vector.y;
        }
    }


    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER && projectile_handler->can_spawn() ){
        Vector2d vec;

        vec.created_at = curr_time;
        vec.x = vec.x_i =  player.weapon.vector.x;
        vec.y = vec.y_i = player.weapon.vector.y;
        vec.set_angle(player.weapon.vector.get_angle());
        vec.direction =player.weapon.vector.direction;

        float rad = PI/180.0f * vec.get_angle();
        vec.vel_x = cos(rad) * 500;
        vec.vel_y = sin(rad) * 500;

        projectile_handler->spawn(vec, rocket);
    }
}

void PlayerHandler::update_physics() {
    float time = (int)(player.jump_time - curr_time)/1000000.0f;
    if (player.vector.vel_y) { //JUMP Physics
        player.vector.y = player.jump_height_at(time);
        if (player.vector.y > noise_map[player.vector.x]) { // End of jump
            player.vector.vel_y = 0;
            player.starting_jump_height = 0;
            player.jump_time = 0;
        }
    }

    // Dont move outside the map
    if (    player.vector.x+player.vector.vel_x > 0 && 
            player.vector.x+player.vector.vel_x <= MAP_SIZE-50  ) {
        player.vector.x+=player.vector.vel_x;
    }

    player.weapon.vector.y = player.vector.y-25;
}

void PlayerHandler::draw(){
    GFX::drawBMP(player.weapon.get_draw_x(), player.weapon.get_draw_y(), 
    player.weapon.vector.get_angle(), CENTER_LEFT, player.vector.direction, 
    0, player.weapon.image);

    GFX::drawBMP(player.get_draw_x(), player.get_draw_y() , 0, CENTER, 
        player.vector.direction, 0, player.image);
}