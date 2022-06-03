#include "object_handler.h"
#include "../logger/logger.h"
#include "../Vector2d.hpp"
#include "../gfx.hpp"

ObjectHandler::ObjectHandler(int MAX_OBJECTS, int _velocity, Image _img)
{
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Calling ObjectList for objects");
    object_list = *(new ObjectList(MAX_OBJECTS));
    Object ** objects = object_list.get_list();

    velocity = _velocity;
    img = _img;
}

ObjectHandler::~ObjectHandler(){
    PSP_LOGGER::log(PSP_LOGGER::DEBUG, "Attempt destruction of Enemy handler with list of size %d", object_list.MAX_SIZE);
    object_list.~ObjectList();
    free(&object_list);
}

void ObjectHandler::spawn(int x, int y, int game_time) {
    if (game_time < last_spawn + time_between_spawns) return;
    if (object_list.size >= object_list.MAX_SIZE){
        //PSP_LOGGER::log(PSP_LOGGER::ERROR, "Spawn fail: Enemy list full");
        return;
    }

    Object * enemy = new Object(img);
    enemy->vector.x = x;
    enemy->vector.y = y;

    PSP_LOGGER::assert((object_list.insert(enemy) > -1), 
        "Object spawned successfully");
    last_spawn = game_time;
}

void ObjectHandler::update_physics(int player_x){
    Object ** enemies = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
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

void ObjectHandler::draw(int cam_pos_x){
    Object ** enemies = object_list.get_list();
    for (int i = 0; i < object_list.MAX_SIZE; i++){
        if (!enemies[i]) continue;
        
        if (!enemies[i]->off_screen()) {
            GFX::drawBMP(enemies[i]->vector.x - cam_pos_x, enemies[i]->vector.y, 
            enemies[i]->vector.get_angle(), CENTER, 
            enemies[i]->vector.direction, 0, img);
        }

    }
}

