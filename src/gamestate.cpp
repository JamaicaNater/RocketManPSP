#include <pspkernel.h>

#include "gamestate.h"
#include "gfx.hpp"
#include "utils.h"
#include "logger/logger.h"

void GameState::init(unsigned char * _noise_map, int _MAP_SIZE){
    player.vector.x = 10;
	player.vector.y = 10;
	player.vector.direction = FORWARD;

    noise_map = _noise_map;
    MAP_SIZE = _MAP_SIZE;

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}
void GameState::update_game_time(int _game_time){
    game_time = _game_time;
}
void GameState::update(){
    cam_pos_x = get_cam_position(player.vector.x, screen_center, MAP_SIZE);
    sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 

    if (MAP_SIZE - player.vector.x > screen_center){ 
        if (player.vector.x > screen_center) player_draw_pos_x = screen_center;
        else player_draw_pos_x = player.vector.x;
    }
    else player_draw_pos_x = player.vector.x - cam_pos_x;

    player.vector.vel_x = 0;
    if(ctrlData.Buttons & PSP_CTRL_LEFT){
        player.vector.vel_x = -1*PLAYER_SPEED;
        player.vector.direction = BACKWARD;
    } 		

    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player.vector.vel_x = PLAYER_SPEED;
        player.vector.direction = FORWARD;
    }

    if(ctrlData.Buttons & PSP_CTRL_UP){ 
        player.vector.set_angle(player.vector.get_angle()-2);
    }
    if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
        player.vector.set_angle(player.vector.get_angle()+2);
    }

    //TODO: Clean up code for readability


    if (!player.jumping) player.vector.y = (int)noise_map[player.vector.x];

    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        if (player.vector.vel_y == 0) {
            player.vector.vel_y= 90; // you cant double jump
            player.jump_time = sceKernelGetSystemTimeLow();
            player.jumping = true;
            player.starting_jump_height = player.vector.y;
        }
    }

        float time = (int)(player.jump_time - game_time)/1000000.0f;
    if (player.jumping) {
        PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG,"calcing with time: %f", time);
        player.vector.y = player.starting_jump_height + player.vector.vel_y*(time*2) + .5 * (player.grav * (time*2) * (time*2) );
        if (player.vector.y > noise_map[player.vector.x]) {
            player.jumping = false;
            player.vector.vel_y = 0;
            player.starting_jump_height = 0;
            player.jump_time = 0;
        }
    }

    if (player.vector.vel_x > 0) {
        if (player.vector.x + PLAYER_SPEED <= MAP_SIZE-50) player.vector.x+=player.vector.vel_x;
    } else if (player.vector.vel_x < 0) {
        if (player.vector.x - PLAYER_SPEED >= 0) player.vector.x += player.vector.vel_x;
    }



}
void GameState::draw(){
    GFX::drawTerrain(noise_map, cam_pos_x);
    GFX::drawBMP(player_draw_pos_x+5, player.vector.y-20, player.vector.get_angle(), CENTER_LEFT, player.vector.direction, "assets/player_rocket.bmp", 0, player.weapon);
    GFX::drawBMP(player_draw_pos_x, player.vector.y , 0, CENTER, player.vector.direction, "assets/player.bmp", 0, player.image);
    

    GFX::swapBuffers();
    GFX::clear();
}
