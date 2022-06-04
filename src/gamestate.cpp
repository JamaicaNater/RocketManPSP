#include <pspkernel.h>
#include <math.h>

#include "gamestate.h"
#include "gfx.hpp"
#include "utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"


void GameState::init(){
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Init Gamestate");

    //TODO: load all BMPs hear and add a loading screen for Christ sake
    load_BMP(enemy_img);
    load_BMP(explosion_animation);

    explosion_handler.init();

    player.vector.x = 10;
	player.vector.y = 10;
	player.vector.direction = FORWARD;
    player.weapon.image = Image("assets/player_rocket.bmp");

    enemy.vector.x = 200;
    enemy.vector.y = noise_map[enemy.vector.x];

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    load_BMP(rocket);
    load_BMP(explosion_animation);
}

void GameState::title_screen() {
    unsigned int home_thid = sceKernelCreateThread("homescreen_thread", GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);
	if (home_thid >= 0) sceKernelStartThread(home_thid, 0, NULL);
	else PSP_LOGGER::log(PSP_LOGGER::ERROR, "failed to create thread");

	while (1)
	{
		sceCtrlReadBufferPositive(&ctrlData, 1);
		if (ctrlData.Buttons & PSP_CTRL_START){
			sceKernelTerminateThread(home_thid);
			sceKernelDeleteThread(home_thid);
			break;
		}
		sceKernelDelayThread(100);
	}
}

void GameState::update(int _game_time){
    game_time = _game_time;

    GameState::update_player_actions();

    GameState::update_nonplayer_actions();

    GameState::update_physics();
}

void GameState::update_nonplayer_actions() {
    enemy_handler.spawn(Vector2d(300, noise_map[300]), game_time, enemy_img);
}

void GameState::update_player_actions() {
    player.vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    cam_pos_x = get_cam_position(player.vector.x, screen_center);
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
    if (!player.vector.vel_y) player.vector.y = (int)noise_map[player.vector.x];
    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        if (player.vector.vel_y == 0) {
            player.vector.vel_y= 50; // you cant double jump
            player.jump_time = sceKernelGetSystemTimeLow();
            player.starting_jump_height = player.vector.y;
        }
    }

    // TODO: if_spawnable();
    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER && game_time >= player.time_between_fires + player.weapon_last_fired ) {
        Vector2d vec;
        player.weapon_last_fired = game_time;

        vec.created_at=sceKernelGetSystemTimeLow();
        vec.x = vec.x_i =  player.weapon.vector.x;
        vec.y = vec.y_i = player.weapon.vector.y;
        vec.set_angle(player.weapon.vector.get_angle());
        vec.direction =player.weapon.vector.direction;

        float rad = PI/180.0f * vec.get_angle();
        vec.vel_x = cos(rad) * 500;
        vec.vel_y = sin(rad) * 500;

        projectile_handler.spawn(vec, game_time, rocket);
    }
}

void GameState::update_physics(){

    projectile_handler.update_physics(game_time);    

    float time = (int)(player.jump_time - game_time)/1000000.0f;
    if (player.vector.vel_y) { //JUMP Physics
        player.vector.y = player.jump_height_at(time);
        if (player.vector.y > noise_map[player.vector.x]) { // End of jump
            player.vector.vel_y = 0;
            player.starting_jump_height = 0;
            player.jump_time = 0;
        }
    }

    // DOnt move outside the map
    if (player.vector.x+player.vector.vel_x > 0 && player.vector.x+player.vector.vel_x <= MAP_SIZE-50) player.vector.x+=player.vector.vel_x;
    enemy.vector.x+=enemy.vector.vel_x;
    enemy.vector.y = noise_map[enemy.vector.x];

    enemy_handler.update_movement(player.vector.x);
    enemy_handler.update_physics();

    player.weapon.vector.x = player.vector.x;
    player.weapon.vector.y = player.vector.y-25;
}

void GameState::draw(){
    GFX::drawTerrain(noise_map, cam_pos_x);
    GFX::simple_drawBMP(0, 272-64-2,  status_bar);
    GFX::draw_progress_bar(50, 240, 20, 120, 80, 100, 0xFF00FF00, 0xFF0000FF);
    GFX::drawBMP(player.weapon.draw_pos_x, player.weapon.vector.y, player.weapon.vector.get_angle(), CENTER_LEFT, player.vector.direction, 0, player.weapon.image);
    GFX::drawBMP(player.draw_pos_x, player.vector.y , 0, CENTER, player.vector.direction, 0, player.image);

    projectile_handler.draw(cam_pos_x);
    explosion_handler.draw(cam_pos_x);

    for (int i = 0; i < explosion_list.MAX_SIZE; i++) {
        if (explosions[i]) {
            if (game_time > explosions[i]->last_frame_update + explosion_animation.frame_time){
                explosions[i]->current_frame++;
                if (explosions[i]->current_frame >= explosion_animation.rows * explosion_animation.cols) {
                    explosion_list.remove(explosions[i]);
                    free(explosions[i]);
                    continue;
                }
                
            }
            Image img = explosion_animation.get_frame(explosions[i]->current_frame);
            GFX::drawBMP(explosions[i]->vector.x - cam_pos_x, explosions[i]->vector.y, explosions[i]->vector.get_angle(), CENTER, FORWARD, 0, img);
        }
    }

    enemy_handler.draw(cam_pos_x);
    //GFX::drawBMP(enemy.vector.x - cam_pos_x, enemy.vector.y, enemy.vector.get_angle(), CENTER, enemy.vector.direction, 0, enemy_img);

    GFX::swapBuffers();
    GFX::clear();
}