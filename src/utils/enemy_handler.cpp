#include "enemy_handler.h"
#include "../logger/logger.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"

EnemyHandler:: EnemyHandler(int MAX_ENEMIES, int _MAP_SIZE, unsigned char * _NOISE_MAP)
{
    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Calling ObjectList for enemy");
    enemy_list = *(new ObjectList(MAX_ENEMIES));
    Object ** enemies = enemy_list.get_list();
    
    NOISE_MAP = _NOISE_MAP;
    MAP_SIZE = _MAP_SIZE;
}

EnemyHandler::~EnemyHandler(){
    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Attempt destruction of Enemy handler of size %d", enemy_list.MAX_SIZE);
    enemy_list.~ObjectList();
    free(&enemy_list);
}

void EnemyHandler::spawn_enemy(int x, int y, int game_time) {
    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "attempting to spawn");
    //if (game_time < last_spawn + time_between_spawns) return;

    Object * enemy = new Object(enemy_img);
    enemy->vector.x = x;
    enemy->vector.y = y;

    PSP_LOGGER::assert_or_log((enemy_list.insert(enemy) > -1), 
        "Enemy spawned successfully");
    last_spawn = game_time;
    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Success spawn");
}

void EnemyHandler::update_physics(int playerx){
    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Attempt update physics");
    Object ** enemies = enemy_list.get_list();
    for (int i = 0; i < enemy_list.MAX_SIZE; i++){
        if (enemies[i]) {
            if (playerx < enemies[i]->vector.x) {
                enemies[i]->vector.vel_x = 1;
                enemies[i]->vector.direction = FORWARD;
            } else {
                enemies[i]->vector.vel_x = -1;
                enemies[i]->vector.direction = BACKWARD;
            }

            enemies[i]->vector.x += enemies[i]->vector.vel_x;
            enemies[i]->vector.y = NOISE_MAP[enemies[i]->vector.x];
        }
    }

    PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Phyisics Updated");
}

void EnemyHandler::draw(int cam_pos_x){
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "handling enemy draw");
    Object ** enemies = enemy_list.get_list();
    for (int i = 0; i < enemy_list.MAX_SIZE; i++){
        if (enemies[i]){
            PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "%d", i);
            GFX::drawBMP(enemies[i]->vector.x - cam_pos_x, enemies[i]->vector.y, enemies[i]->vector.get_angle(), CENTER, enemies[i]->vector.direction, 0, enemy_img);
        }
    }
}

