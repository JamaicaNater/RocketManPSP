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

    Person player = Person(Image("assets/player.bmp"));
    Image rocket = Image("assets/missile.bmp");

    Image exp_frame = Image(""); // TODO: fix error prone nature of this constructer

    Object * projectiles[MAX_PROJ] = {NULL};
    int num_projectiles = 0;
    
    unsigned int game_time = 0;

    int cam_pos_x =10, cam_pos_y =10;
	int screen_center = 512/2;

    SceCtrlData ctrlData;

    void update_player_actions();
    void update_pyhsics();
public:
    /**
     * @brief Initialize variabless needed for gamestate, 
     * prepare sytem for reading controls.
     * 
     * @param _noise_map 
     * @param _MAP_SIZE 
     */
    void init(unsigned char * _noise_map, int _MAP_SIZE);

    /**
     * @brief Update player controled actions (controller input),
     * player uncontrolled actions (AI) and Physics 
     * 
     * @param _game_time
     */
    void update(int _game_time);

    /**
     * @brief Draw objects present in the game context
     * 
     */
    void draw();
    inline GameState();
    inline ~GameState();
};

GameState::GameState(){

}

GameState::~GameState(){
    
}