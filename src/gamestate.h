#pragma once

#include <pspctrl.h>
#include "objects.hpp"
#include "utils/object_list.hpp"

// TODO make singleton
class GameState
{
private:
    const int PLAYER_SPEED = 2;
    static const int MAX_PROJ = 64;
    static const int MAX_ENEMIES = 16;

    int MAP_SIZE;
    unsigned char * noise_map;

    Person player = Person(Image("assets/player.bmp"));
    Image rocket = Image("assets/missile.bmp");
    Image status_bar = Image("assets/statusbar.bmp");
    Image enemy_img = Image("assets/enemy.bmp");

    Object enemy;
    Object * enemies[MAX_ENEMIES];
    ObjectList projectile_list = ObjectList(MAX_ENEMIES);
    Object * * projectiles = projectile_list.get_list();
    ObjectList explosion_list = ObjectList(128); // TODO think about what the upper bound should be
    Object * * explosions =  explosion_list.get_list();

    Animation explosion_animation = Animation(3, 5, 50000, "assets/explosion.bmp");
    //Object exp_obj = Object(""); // TODO: fix error prone nature of this constructor
    
    unsigned int game_time = 0;

    int cam_pos_x =10, cam_pos_y =10;
	int screen_center = 512/2;

    SceCtrlData ctrlData;

    /**
     * @brief Responsible for handling actions not directly caused by the player ie.
     * certain animations, enemies and damage and collision events
     * 
     */
    void update_nonplayer_actions();

    /**
     * @brief Responsible for handling player input, partially responsible for some
     * physics calculations
     * 
     */
    void update_player_actions();


    void update_physics();
public:
    void title_screen();

    /**
     * @brief Initialize variables needed for GameState, 
     * prepare system for reading controls.
     * 
     * @param _noise_map 
     * @param _MAP_SIZE 
     */
    void init(unsigned char * _noise_map, int _MAP_SIZE);

    /**
     * @brief Update player controlled actions (controller input),
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