#include <pspkernel.h>
#include <math.h>
#include <pspctrl.h>
#include <random>

#include <graphics/game_graphics.h>
#include <gfx.hpp>
#include <logger.h>
#include <loadbmp.h>
#include <object_handler/global_object_manager.h>

#include "gamestate.h"
#include "game_globals.h"
#include "game_utils.h"

#include "interface/pause_menu.h"
#include "interface/title_menu.h"
#include "interface/main_menu.h"


GameState::StatusInfo GameState::status_info = {GameState::RUNNING, sceKernelGetSystemTimeLow()};

bool GameState::in_title = false;
bool GameState::in_menu = false;

int GameState::enemies_killed = 0;

uint32_t GameState::time_game_start = 0;

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
    srand(sceKernelGetSystemTimeLow());
}

void GameState::exit_game(){
    sceKernelExitGame();
}

void GameState::title_screen() {
    in_title = true;
    Menu title_menu = build_title_menu();
    title_menu.open();
}

void GameState::menu_screen() {
    in_menu = true;
    Menu main_menu = build_main_menu();
    main_menu.open();

    time_game_start = sceKernelGetSystemTimeLow();
}

void GameState::on_pause(){
    Menu pause_menu = build_pause_menu();
    pause_menu.open();
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
    int spawn_pos_x;
    do {
        spawn_pos_x = rand() % MAP_SIZE;
    } while (spawn_pos_x < PlayerHandler::player->vector.x + 50 && spawn_pos_x > PlayerHandler::player->vector.x - 50);
    enemy_handler.spawn(Vector2d(spawn_pos_x, noise_map[spawn_pos_x]), enemy_img);

    explosion_handler.update_frames();

    update_kills(kill_counter);
    kill_counter.update();

    update_game_time_icon(game_time_icon);
    game_time_icon.update();
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
    drawTerrain();
    GFX::simple_drawBMP(0, SCREEN_HEIGHT-64-2,  status_bar);
    GFX::draw_progress_bar(50, 240, 20, 120,
        PlayerHandler::get_player_val().health,
        PlayerHandler::get_player_val().max_health, 0xFF00FF00, 0xFF0000FF);

    projectile_handler.draw();
    player_handler.draw();
    enemy_handler.draw();
    explosion_handler.draw();

    kill_counter.draw();
    game_time_icon.draw();

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