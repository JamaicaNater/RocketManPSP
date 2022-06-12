#pragma once

#include "globals.h"
#include "objects.hpp"
#include "object_handler/object_list.h"
#include "object_handler/object_handler.h"
#include "object_handler/enemy_handler.h"
#include "object_handler/animation_handler.h"
#include "object_handler/projectile_handler.h"
#include "object_handler/player_handler.h"
#include "image_builder/component.hpp"



// TODO make singleton
class GameState
{
private:
    const int PLAYER_SPEED = 2;
    static const int MAX_PROJ = 64;
    static const int MAX_ENEMIES = 16;

    Object player = Object(Image("assets/player.bmp"));
    Image rocket = Image("assets/missile.bmp");
    Image status_bar = Image("assets/statusbar.bmp");
    Image enemy_img = Image("assets/enemy.bmp");

    
    
    EnemyHandler enemy_handler = EnemyHandler(MAX_ENEMIES, 1, 1*SECOND, 
        Object::ENEMY);
    AnimationHandler explosion_handler = AnimationHandler(128, 0, 0, 
        Object::EXPLOSION);

    ProjectileHandler projectile_handler = 
        ProjectileHandler(MAX_PROJ, 8, 300 * MILLISECOND, Object::MISSILE, 
        &explosion_handler);

    PlayerHandler player_handler = PlayerHandler(PLAYER_SPEED, &projectile_handler);

    Component comp = Component(20,20,45,45, 0xFF0000FF);

	int screen_center = SCREEN_WIDTH/2;
    SceCtrlData ctrlData;

    /**
     * @brief Responsible for handling actions not directly caused by the player
     * ie. certain animations, enemies and damage and collision events
     * 
     */
    void update_nonplayer_actions();

    /**
     * @brief Responsible for handling player input, partially responsible for 
     * some physics calculations
     * 
     */
    void update_player_actions();

    void update_physics();
public:
    enum GameStates{RUNNING, PAUSED, TERMINATED};
    static GameStates state;
    void title_screen();

    /**
     * @brief Initialize variables needed for GameState, 
     * prepare system for reading controls.
     */
    void init();

    /**
     * @brief Update player controlled actions (controller input),
     * player uncontrolled actions (AI) and Physics 
     * 
     */
    void update();

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