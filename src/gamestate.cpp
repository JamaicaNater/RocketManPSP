#include <pspkernel.h>

#include "gamestate.h"
#include "gfx.hpp"
#include "utils.h"
#include "logger/logger.h"

#include "bmp/loadbmp.h"

void GameState::init(unsigned char * _noise_map, int _MAP_SIZE){
    player.vector.x = 10;
	player.vector.y = 10;
	player.vector.direction = FORWARD;

    proj.vector.x = 10;
    proj.vector.y = 10;
    proj.vector.direction = FORWARD;

    noise_map = _noise_map;
    MAP_SIZE = _MAP_SIZE;

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}
void GameState::update_game_time(int _game_time){
    game_time = _game_time;
}
void GameState::update(){
    player.vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    cam_pos_x = get_cam_position(player.vector.x, screen_center, MAP_SIZE);
    sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 

    // Where to draw the player on the screen
    if (MAP_SIZE - player.vector.x > screen_center){ 
        if (player.vector.x > screen_center) player.draw_pos_x = screen_center;
        else player.draw_pos_x = player.vector.x;
    }
    else player.draw_pos_x = player.vector.x - cam_pos_x;

    // PLAYER MOVEMENT
    if(ctrlData.Buttons & PSP_CTRL_LEFT){
        player.vector.vel_x = -1*PLAYER_SPEED;
        player.vector.direction = BACKWARD;
    } 		
    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player.vector.vel_x = PLAYER_SPEED;
        player.vector.direction = FORWARD;
    }

    // Weapon angle positioning
    if(ctrlData.Buttons & PSP_CTRL_UP){ 
        player.weapon.vector.set_angle(player.weapon.vector.get_angle()-2);
    }
    if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
        player.weapon.vector.set_angle(player.weapon.vector.get_angle()+2);
    }

    // Jump 
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
    if (player.jumping) { //JUMP Physics
        PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG,"calcing with time: %f", time);
        player.vector.y = player.starting_jump_height + player.vector.vel_y*(time*2) + .5 * (player.grav * (time*2) * (time*2) );
        if (player.vector.y > noise_map[player.vector.x]) {
            player.jumping = false;
            player.vector.vel_y = 0;
            player.starting_jump_height = 0;
            player.jump_time = 0;
        }
    }

    // TODO: do we need nested ifs?
    if (player.vector.vel_x > 0) {
        if (player.vector.x + PLAYER_SPEED <= MAP_SIZE-50) player.vector.x+=player.vector.vel_x;
    } else if (player.vector.vel_x < 0) {
        if (player.vector.x - PLAYER_SPEED >= 0) player.vector.x += player.vector.vel_x;
    }
}
void GameState::draw(){
    GFX::drawTerrain(noise_map, cam_pos_x);
    GFX::drawBMP(player.draw_pos_x+5, player.vector.y-20, player.weapon.vector.get_angle(), CENTER_LEFT, player.vector.direction, "assets/player_rocket.bmp", 0, player.weapon.image);
    GFX::drawBMP(player.draw_pos_x, player.vector.y , 0, CENTER, player.vector.direction, "assets/player.bmp", 0, player.image);

    for (int i = 0; i < num_projectiles; i++){
        if (projectiles[i]) GFX::drawBMP(int(projectiles[i]->vector.x - cam_pos_x), projectiles[i]->vector.y, 0, CENTER, projectiles[i]->vector.direction, "assets/missile.bmp", 0, projectiles[i]->image);
    }

    GFX::swapBuffers();
    GFX::clear();
}
