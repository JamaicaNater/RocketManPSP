#include <pspkernel.h>
#include <math.h>
#include <pspctrl.h>

#include "gamestate.h"
#include "graphics/gfx.hpp"
#include "utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"
#include "object_handler/global_object_manager.h"
#include "image_builder/text_builder.h"


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
    init_text_builder();
    
    load_BMP(rocket);
    load_BMP(status_bar);
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
            sceKernelDelayThread(200 * MILLISECOND); // So that we have 
            //time for the start(pause) button to be released
			break;
		}
		sceKernelDelayThread(100);
	}
}

void GameState::on_pause(){
    uint32_t pause_time_begin = sceKernelGetSystemTimeLow(); 
        
    Image img = pause_menu.get_image();
    img2 = text("Game Paused");
    pause_menu.set_pos(CENTER, 0, -20);
    pause_menu.add_panel(BOTTOM_CENTER, 10, 20, 0x00FF00);
    pause_menu.add_img(00,00, img2);
    
    GFX::blur_screen();
    GFX::simple_drawBMP(pause_menu.x, pause_menu.y, img);
    GFX::swapBuffers();

    wait_button_release(ctrlData, PSP_CTRL_START);   
    while (1)
    {
        sceCtrlReadBufferPositive(&ctrlData, 1);
        if (ctrlData.Buttons & PSP_CTRL_START){
            state = RUNNING;
            
            wait_button_release(ctrlData, PSP_CTRL_START);
            uint32_t pause_time_end = sceKernelGetSystemTimeLow();  
            pause_time += (pause_time_end - pause_time_begin);
            psp_free(img2.img_matrix);
            break;
        }
    }
}

void GameState::update(){
    if (state == PAUSED) { 
        GameState::on_pause();
    }

    curr_time = get_time();
    GameState::update_player_actions();

    curr_time = get_time();
    GameState::update_nonplayer_actions();

    curr_time = get_time();
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
    GFX::draw_progress_bar(50, 240, 20, 120, 
        PlayerHandler::get_player_val().health, 
        PlayerHandler::get_player_val().max_health, 0xFF00FF00, 0xFF0000FF);
    
    projectile_handler.draw();
    player_handler.draw();
    enemy_handler.draw();
    explosion_handler.draw();

    ObjectManager::draw_health_bars();
    //GFX::tint_screen(0x000000);
       
    GFX::swapBuffers();
    GFX::clear();
}