#include <pspkernel.h>
#include <math.h>
#include <pspctrl.h>

#include "gamestate.h"
#include "graphics/gfx.hpp"
#include "utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"
#include "object_handler/global_object_manager.h"


GameState::GameStates GameState::state = GameState::RUNNING;

void GameState::init(){
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Init Gamestate");
    state = RUNNING;
    camera_x = 0;
    player_handler.init();

    //TODO: load all BMPs here
    
    load_BMP(enemy_img);

    explosion_handler.init(new Animation(3, 5, 50000, "assets/explosion.bmp"));
    
    player.vector.x = 10;
	player.vector.y = 10;
	player.vector.direction = FORWARD;
    player.type = Object::PLAYER;

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    comp.add_panel(10,10, 10, 20, 0xFF00FF00);

    load_BMP(rocket);
    load_BMP(status_bar);
    //load_BMP(font);

}

void GameState::title_screen() {
    int home_thid = sceKernelCreateThread("homescreen_thread", 
        GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);

	if (home_thid >= 0) sceKernelStartThread(home_thid, 0, NULL);
	else PSP_LOGGER::log(PSP_LOGGER::ERROR, "failed to create thread");

	while (1)
	{
		sceCtrlReadBufferPositive(&ctrlData, 1);
		if (ctrlData.Buttons & PSP_CTRL_START){
			sceKernelTerminateThread(home_thid);
			sceKernelDeleteThread(home_thid);
            sceKernelDelayThread(100 * MILLISECOND); // So that we have 
            //time for the start(pause) button to be released
			break;
		}
		sceKernelDelayThread(100);
	}
}

void GameState::update(){
    if (state == PAUSED) {
        sceKernelDelayThread(200 * MILLISECOND); // So that we have 
                //time for the start(pause) button to be released
        while (1)
        {
            sceCtrlReadBufferPositive(&ctrlData, 1);
            if (ctrlData.Buttons & PSP_CTRL_START){
                state = RUNNING;
                sceKernelDelayThread(200 * MILLISECOND); // So that we have 
                //time for the start(pause) button to be released
                break;
            }
            sceKernelDelayThread(100);
        }
    }

    curr_time = sceKernelGetSystemTimeLow();
    GameState::update_player_actions();

    curr_time = sceKernelGetSystemTimeLow();
    GameState::update_nonplayer_actions();

    curr_time = sceKernelGetSystemTimeLow();
    GameState::update_physics();
}

void GameState::update_nonplayer_actions() {
    enemy_handler.spawn(Vector2d(300, noise_map[300]), enemy_img);
    explosion_handler.update_frames();
}

void GameState::update_player_actions() {
    player_handler.read_controls();
}

void GameState::update_physics(){  
    player_handler.update_physics();
    projectile_handler.update_physics();  
    enemy_handler.update_movement(PlayerHandler::get_player_val().vector.x);
    enemy_handler.update_physics();
    explosion_handler.update_physics();
}

void GameState::draw(){
    GFX::drawTerrain();
    GFX::simple_drawBMP(0, SCREEN_HEIGHT-64-2,  status_bar);
    GFX::draw_progress_bar(50, 240, 20, 120, PlayerHandler::get_player_val().health
        , PlayerHandler::get_player_val().max_health, 0xFF00FF00, 0xFF0000FF);
    
    projectile_handler.draw();
    player_handler.draw();
    enemy_handler.draw();
    explosion_handler.draw();

    ObjectManager::draw_health_bars();

    
    // Image img = comp.get_image();
    // GFX::simple_drawBMP(20, 50, img);


    // img = font.get_frame(1);
    
    // GFX::simple_drawBMP(20, 50, img);
    //sceKernelDelayThread(1 * SECOND);

    GFX::swapBuffers();
    GFX::clear();
}