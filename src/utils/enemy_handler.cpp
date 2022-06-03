#include "enemy_handler.h"
#include "../logger/logger.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"

EnemyHandler:: EnemyHandler(int MAX_ENEMIES, int _velocity, Image _img)
{
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Calling ObjectList for enemy");
    enemy_list = *(new ObjectList(MAX_ENEMIES));
    Object ** enemies = enemy_list.get_list();

    velocity = _velocity;
    img = _img;
}

EnemyHandler::~EnemyHandler(){
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Attempt destruction of Enemy handler with list of size %d", enemy_list.MAX_SIZE);
    enemy_list.~ObjectList();
    free(&enemy_list);
}

void EnemyHandler::spawn_enemy(int x, int y, int game_time) {
    if (game_time < last_spawn + time_between_spawns) return;
    if (enemy_list.size >= enemy_list.MAX_SIZE){
        //PSP_LOGGER::log(PSP_LOGGER::ERROR, "Spawn fail: Enemy list full");
        return;
    }

    Object * enemy = new Object(img);
    enemy->vector.x = x;
    enemy->vector.y = y;

    PSP_LOGGER::assert((enemy_list.insert(enemy) > -1), 
        "Enemy spawned successfully");
    last_spawn = game_time;
}

void EnemyHandler::update_physics(int player_x){
    Object ** enemies = enemy_list.get_list();
    for (int i = 0; i < enemy_list.MAX_SIZE; i++){
        if (!enemies[i]) continue;

        if (player_x < enemies[i]->vector.x) {
        enemies[i]->vector.vel_x = -1 * velocity;
        enemies[i]->vector.direction = FORWARD;
        } else {
            enemies[i]->vector.vel_x = 1 * velocity;
            enemies[i]->vector.direction = BACKWARD;
        }

        enemies[i]->vector.x += enemies[i]->vector.vel_x;
        enemies[i]->vector.y = noise_map[enemies[i]->vector.x];
    }
}

void EnemyHandler::draw(int cam_pos_x){
    Object ** enemies = enemy_list.get_list();
    for (int i = 0; i < enemy_list.MAX_SIZE; i++){
        if (!enemies[i]) continue;
        
        if (!enemies[i]->off_screen()) {
            GFX::drawBMP(enemies[i]->vector.x - cam_pos_x, enemies[i]->vector.y, 
            enemies[i]->vector.get_angle(), CENTER, 
            enemies[i]->vector.direction, 0, img);
        }

    }
}

