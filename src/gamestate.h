#pragma once

#include <pspctrl.h>
#include "Projectile.hpp"

class GameState
{
private:
    Person player;
	bool cam_aligned = true;
    
    unsigned int game_time = 0;

    int cam_pos_x =10, cam_pos_y =10;
    const int PLAYER_SPEED = 2;

    int player_draw_pos_x = 0;
	bool cam_locked_left = true;
	bool cam_locked_right = false;
	int screen_center = 512/2;

    SceCtrlData ctrlData;

    int MAP_SIZE;
    unsigned char * noise_map;


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