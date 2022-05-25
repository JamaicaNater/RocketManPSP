#pragma once

#include <pspctrl.h>
#include "Projectile.hpp"

class GameState
{
private:
    const int PLAYER_SPEED = 2;
    static const int MAX_PROJ = 64;

    int MAP_SIZE;
    unsigned char * noise_map;

    Person player;
    Object proj;

    Object * projectiles[MAX_PROJ] = {NULL};
    int num_projectiles = 0;
    
    unsigned int game_time = 0;

    int cam_pos_x =10, cam_pos_y =10;
    int player_draw_pos_x = 0;
	int screen_center = 512/2;

    SceCtrlData ctrlData;

    /* data */
public:
    unsigned int time;
    void init(unsigned char * _noise_map, int _MAP_SIZE);
    void update_game_time(int _game_time);
    void update();
    void draw();
    inline GameState();
    inline ~GameState();
};

GameState::GameState(){

}

GameState::~GameState(){
    
}