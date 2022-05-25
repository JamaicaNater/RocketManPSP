#include <pspkernel.h>
#include <math.h>

#include "gamestate.h"
#include "gfx.hpp"
#include "utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"


void GameState::init(unsigned char * _noise_map, int _MAP_SIZE){
    player.vector.x = 10;
	player.vector.y = 10;
	player.vector.direction = FORWARD;

    noise_map = _noise_map;
    MAP_SIZE = _MAP_SIZE;

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    load_BMP(&rocket.height, &rocket.width, rocket.img_matrix, "assets/missile.bmp");
}

void GameState::update(int _game_time){
    game_time = _game_time;

    GameState::update_player_actions();

    GameState::update_pyhsics();
}
void GameState::draw(){
    GFX::drawTerrain(noise_map, cam_pos_x);
    GFX::drawBMP(player.weapon.draw_pos_x, player.weapon.vector.y, player.weapon.vector.get_angle(), CENTER_LEFT, player.vector.direction, "assets/player_rocket.bmp", 0, player.weapon.image);
    GFX::drawBMP(player.draw_pos_x, player.vector.y , 0, CENTER, player.vector.direction, "assets/player.bmp", 0, player.image);

    for (int i = 0; i < num_projectiles; i++){
        if (projectiles[i]) {
            PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "rocket: x: %d, y: %d",int(projectiles[i]->vector.x - cam_pos_x), projectiles[i]->vector.y);
            GFX::drawBMP(projectiles[i]->draw_pos_x, projectiles[i]->vector.y, projectiles[i]->vector.get_angle(), CENTER, projectiles[i]->vector.direction, "assets/missile.bmp", 0, rocket);
        }
    }
    GFX::swapBuffers();
    GFX::clear();
}

/**
 * @brief Resposible for handling player input, partially responsible for some
 * physics calculations
 * 
 */
void GameState::update_player_actions() {
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
        player.weapon.draw_pos_x = player.draw_pos_x-10;
    } 		
    if(ctrlData.Buttons & PSP_CTRL_RIGHT){
        player.vector.vel_x = PLAYER_SPEED;
        player.vector.direction = FORWARD;
        player.weapon.draw_pos_x = player.draw_pos_x+10;
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

    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER && num_projectiles < 1) {
        Object * proj = new Object();
        projectiles[num_projectiles] = proj;
        num_projectiles++;

        proj->vector.x = player.weapon.vector.x;
        proj->vector.y = player.weapon.vector.y;
        proj->vector.set_angle(player.weapon.vector.get_angle());
        proj->vector.direction =player.weapon.vector.direction;

        float rad = PI/180.0f * proj->vector.get_angle();
        proj->vector.vel_x = cos(rad) * 10;
        proj->vector.vel_y = sin(rad) * 10;
    }
}

void GameState::update_pyhsics(){

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

    // DOnt move outside the map
    if (player.vector.x+player.vector.vel_x > 0 && player.vector.x+player.vector.vel_x <= MAP_SIZE-50) player.vector.x+=player.vector.vel_x;

    for (int i = 0; i < num_projectiles; i++){
        if (projectiles[i]) {
            projectiles[i]->vector.y+=projectiles[i]->vector.vel_y;
            projectiles[i]->vector.x+=projectiles[i]->vector.vel_x;
            projectiles[i]->draw_pos_x+=projectiles[i]->vector.vel_x;
            projectiles[i]->draw_pos_x = projectiles[i]->vector.x - cam_pos_x;

            if (projectiles[i]->draw_pos_x > SCREEN_WIDTH_RES + 50 || projectiles[i]->draw_pos_x < -50
                || projectiles[i]->vector.y > SCREEN_HEIGHT + 50 || projectiles[i]->vector.y < -50) {
                    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Freed projectile %d located at scr_x%d, y%d",i, projectiles[i]->draw_pos_x, projectiles[i]->vector.y);
                    free(projectiles[i]); // TODO num_projectiles?
                    num_projectiles--;
                }
        }
    }

    player.weapon.vector.x = player.vector.x;
    player.weapon.vector.y = player.vector.y-25;
}