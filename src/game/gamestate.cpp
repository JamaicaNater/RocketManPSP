#include <pspkernel.h>
#include <math.h>
#include <pspctrl.h>

#include "gamestate.h"
#include "graphics/gfx.hpp"
#include "game_utils.h"
#include "logger/logger.h"
#include "bmp/loadbmp.h"
#include "object_handler/global_object_manager.h"

#include "interface/pause_menu.h"
#include "interface/title_menu.h"



GameState::StatusInfo GameState::status_info = {GameState::RUNNING, sceKernelGetSystemTimeLow()};

bool GameState::in_title = true;

void GameState::init(){
    log(INFO, "Init Gamestate");
    GameState::update_status(RUNNING);
    //in_menu = true;
    in_title = true;
    camera_x = 0;

    //TODO: load all BMPs here
    load_BMP(status_bar);
    load_BMP(enemy_img);
    
    player_handler.init();
    explosion_handler.init(new Animation(3, 5, 50000, "assets/misc/explosion.bmp"));

    control_reader.init();
    init_text_builder();    
}

void GameState::exit_game(){
    sceKernelExitGame();
}

void GameState::title_screen() {
    Menu title_menu = build_title_menu();
    title_menu.on_open(title_menu);
}

void GameState::on_pause(){    
    Menu pause_menu = build_pause_menu();
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