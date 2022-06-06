#pragma once

#include "globals.h"
#include <pspctrl.h>
#include "objects.hpp"
#include "utils/object_list.hpp"
#include "utils/object_handler.h"
#include "utils/enemy_handler.h"
#include "utils/animation_handler.h"
#include "utils/projectile_handler.h"

// TODO make singleton
class GameState
{
private:
    const int PLAYER_SPEED = 2;
    static const int MAX_PROJ = 64;
    static const int MAX_ENEMIES = 16;

    Person player = Person(Image("assets/player.bmp"));
    Image rocket = Image("assets/missile.bmp");
    Image status_bar = Image("assets/statusbar.bmp");
    Image enemy_img = Image("assets/enemy.bmp");
    
    EnemyHandler enemy_handler = EnemyHandler(MAX_ENEMIES, 1, 1*SECOND, Object::ENEMY);
    AnimationHandler explosion_handler = AnimationHandler(128, 0, 0, Object::EXPLOSION);
    ProjectileHandler projectile_handler = ProjectileHandler(MAX_PROJ, 8, 80 * MILLISECOND, Object::MISSILE, &explosion_handler);

	int screen_center = SCREEN_WIDTH/2;

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
    enum GameStates{RUNNING, PAUSED, TERMINATED};
    
    GameStates state = RUNNING;
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