#include <pspkernel.h>
#include <math.h>
#include <pspctrl.h>

#include "gamestate.h"
#include "graphics/gfx.hpp"
#include "utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"
#include "object_handler/global_object_manager.h"



GameState::StatusInfo GameState::status_info = {GameState::RUNNING, sceKernelGetSystemTimeLow()};

void GameState::init(){
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Init Gamestate");
    GameState::update_status(RUNNING);
    //in_menu = true;
    in_title = true;
    camera_x = 0;

    //TODO: load all BMPs here
    load_BMP(status_bar);
    load_BMP(enemy_img);
    
    player_handler.init();
    explosion_handler.init(new Animation(3, 5, 50000, "assets/explosion.bmp"));

    control_reader.init();
    init_text_builder();    
}

void GameState::title_screen() {
    int home_thid = sceKernelCreateThread("homescreen_thread", 
        GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);

	if (home_thid >= 0) sceKernelStartThread(home_thid, 0, NULL);
	else PSP_LOGGER::log(PSP_LOGGER::ERROR, "failed to create thread");

    control_reader.on_button_press_start = [home_thid, this](){
        sceKernelTerminateThread(home_thid);
        sceKernelDeleteThread(home_thid);
        sceKernelDelayThread(200 * MILLISECOND); // So that we have 
        //time for the start(pause) button to be released
        in_title = false;
    };

	while (in_title)
	{
        sceKernelDelayThread(100);
        control_reader.read_controls();
	}
}

void GameState::on_pause(){
    control_reader.on_button_press_start = [this](){       
        control_reader.wait_button_release(PSP_CTRL_START);  
        pause_time += (sceKernelGetSystemTimeLow() - GameState::status_info.start_time);
        GameState::update_status(RUNNING);
    };
    
    Menu pause_menu = Menu(CENTER, 120, 90, 0xC0C0C0, 0, -20);
    pause_menu.add_component(BOTTOM_CENTER, 
        Component(10,20, Component::Rectangle, 0x00FF00),
        0, 20);
    pause_menu.add_component(CENTER, Component("Game Paused"));
    pause_menu.update();
    
    pause_menu.on_open = [this](Menu self){
        Image img = self.get_image();
        GFX::blur_screen();
        GFX::simple_drawBMP(self.x, self.y, img);
        GFX::swapBuffers();

        control_reader.wait_button_release(PSP_CTRL_START);   
        while (status_info.status == PAUSED)
        {   
            control_reader.read_controls();
        }
    };

    pause_menu.on_open(pause_menu);
}

void GameState::update(){
    if (status_info.status == PAUSED) { 
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

void GameState::update_status(Status _status){
    status_info.status = _status;
    status_info.start_time = sceKernelGetSystemTimeLow();
}

GameState::GameState(){

}

GameState::~GameState(){
    
}