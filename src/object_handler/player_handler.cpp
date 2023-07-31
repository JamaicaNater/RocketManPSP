#include "player_handler.h"

#include <pspkernel.h>
#include <math.h>

#include "globals.h"

#include "gamestate.h"
#include "game_utils.h"

#include "gfx.hpp"
#include "physics/physics.h"
#include "loadbmp.h"

#include "interface/death_menu.h"

PlayerHandler::PlayerHandler(int _velocity, int * camera_x, 
    unsigned char * terrain, ProjectileHandler * _projectile_handler) :
    ObjectHandler(2, _velocity, 0, camera_x, terrain, Object::PLAYER
) {
    projectile_handler = _projectile_handler;
}

PlayerHandler::~PlayerHandler() {
}

void PlayerHandler::init() {
    // For Controls
    player_control_reader.init();

    load_BMP(rocket);
    load_BMP(weapon->image);
    load_BMP(player->image);

    player->type = Object::PLAYER;
    player->vector.x = MAP_SIZE/2;
    object_list.insert(player);

    weapon->vector.pivot = CENTER_LEFT;
    weapon->type = Object::WEAPON; // TODO Weapon break mechanic?
    object_list.insert(weapon);
    player->health = 1000;
    player->max_health = 1000;

    projectile_handler->init();
    set_up_controls();
}

Object * PlayerHandler::player = new Object(Image("assets/player/player1.bmp"));

Object PlayerHandler::get_player_val(){
    return *player;
}

void PlayerHandler::set_up_controls(){
    player_control_reader.on_button_press_left = [this](){
        player->vector.vel_x = -1*velocity;
        player->vector.direction = BACKWARD;
        weapon->vector.direction = BACKWARD;
        weapon->vector.x = player->vector.x-10;
    };
    player_control_reader.on_button_press_right = [this](){
        player->vector.vel_x = velocity;
        player->vector.direction = FORWARD;
        weapon->vector.direction = FORWARD;
        weapon->vector.x = player->vector.x+10;
    };
    player_control_reader.on_button_press_up = [this](){
        weapon->vector.angle = weapon->vector.angle-2;
    };
    player_control_reader.on_button_press_down = [this](){
        weapon->vector.angle = weapon->vector.angle+2;
    };

    player_control_reader.on_button_press_cross = [this](){
        if (player->vector.vel_y == 0) {
            player->vector.vel_y= -40; // you cant double jump
            player->vector.y_i = player->vector.y;
            player->vector.t0_y = curr_time;
        }
    };

    player_control_reader.on_button_press_r_trig = [this](){
        if (!projectile_handler->can_spawn()) return;
        Vector2d vec;

        vec.t0_y = vec.t0_x = curr_time;
        vec.angle = weapon->vector.angle;
        vec.direction = weapon->vector.direction;
        vec.pivot = weapon->vector.pivot;

        float rad = PI/180.0f * ((player->vector.direction==FORWARD)*vec.angle
            + (player->vector.direction==BACKWARD)*vec.get_mirrored_angle());

        // Add distance so the missile doesnt collide with player/weapon
        vec.x = vec.x_i = weapon->vector.x + cos(rad)*25;
        vec.y = vec.y_i = weapon->vector.y + sin(rad)*25;
        vec.vel_x = cos(rad) * 550;
        vec.vel_y = sin(rad) * 550;

        projectile_handler->spawn(vec, rocket);
    };

    player_control_reader.on_button_press_start = [this](){
        GameState::update_status(GameState::PAUSED);
    };
}

void PlayerHandler::read_controls(){
    player->vector.vel_x = 0;// reset velocity; TODO: slowdown mechanic
    log(INFO, "camera: %d", *camera_x);
    *camera_x = get_cam_position(player->vector.x, SCREEN_WIDTH/2);
    log(INFO, "camera: %d", *camera_x);
    player_control_reader.read_controls();

    // Jump
    if (!player->vector.vel_y) player->vector.y = (int)noise_map[player->vector.x];
}

void PlayerHandler::update_physics() {
    ObjectHandler::update_physics();

    weapon->vector.y = player->vector.y-25;
}

void PlayerHandler::on_off_screen(Object * obj)  {
    if (player->vector.x < 0) player->vector.x = 20;
    if (player->vector.x > MAP_SIZE) player->vector.x = MAP_SIZE - 50;
}

void PlayerHandler::on_zero_health(Object * obj) {
    if(obj->type != Object::PLAYER) return;
    ObjectHandler::on_zero_health(obj);

    Menu death_menu = build_death_menu();
    death_menu.open();
}

