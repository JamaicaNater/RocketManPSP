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
#include "interface/main_menu.h"


GameState::StatusInfo GameState::status_info = {GameState::RUNNING, sceKernelGetSystemTimeLow()};

bool GameState::in_title = false;
bool GameState::in_menu = false;

int GameState::enemies_killed = 0;

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
}

void GameState::exit_game(){
    sceKernelExitGame();
}

void GameState::title_screen() {
    in_title = true;
    Menu title_menu = build_title_menu();
    title_menu.on_open(title_menu);
}

void GameState::menu_screen() {
    in_menu = true;
    Menu main_menu = build_main_menu();
    main_menu.on_open(main_menu);
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

    update_kills(kill_counter);
    kill_counter.update();
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
    kill_counter.draw();

    ObjectManager::draw_health_bars();
    //GFX::tint_screen(0x000000);

    GFX::swapBuffers();
    GFX::clear();
}

void GameState::update_status(Status _status) {
    if (status_info.status == GameState::PAUSED) {
        pause_time += (sceKernelGetSystemTimeLow() - GameState::status_info.start_time);
    }
    status_info.status = _status;
    status_info.start_time = sceKernelGetSystemTimeLow();
}

GameState::GameState(){

}

GameState::~GameState(){

}