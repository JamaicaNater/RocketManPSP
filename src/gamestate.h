#pragma once

#include <control_reader.h>

#include "object_handler/enemy_handler.h"
#include "object_handler/animation_handler.h"
#include "object_handler/explosion_handler.h"
#include "object_handler/projectile_handler.h"
#include "object_handler/player_handler.h"
#include "interface/on_screen.h"

// TODO make singleton
class GameState
{
public:
    enum Status{RUNNING, PAUSED, TERMINATED};
    struct StatusInfo
    {
        Status status;
        uint32_t start_time; // when did we enter this state
    };

    static bool in_menu;
    static bool in_title;
    static StatusInfo status_info;

    static int enemies_killed;

    static uint32_t time_game_start;

    Menu kill_counter = build_kill_counter();
    Menu game_time_icon = build_game_time_icon();

    /**
     * @brief Exits the game
     *
     */
    static void exit_game();

    /**
     * @brief Load the title screen image and render it to the screen wait for
     * player to press the buttons
     *
     */
    void title_screen();

    /**
     * @brief menu screen with options
     *
     */
    void menu_screen();

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
     * @brief update Status and set time;
     *
     */
    static void update_status(GameState::Status state);

    /**
     * @brief Draw objects present in the game context
     *
     */
    void draw();

    GameState();
    ~GameState();

private:
    const int PLAYER_SPEED = 2;
    static const int MAX_PROJ = 64;
    static const int MAX_ENEMIES = 16;

    Image status_bar = Image("assets/game/statusbar.bmp");
    Image enemy_img = Image("assets/enemy/enemy.bmp");

    EnemyHandler enemy_handler = EnemyHandler(MAX_ENEMIES, 1, 1*SECOND, &camera_x, noise_map,
        Object::ENEMY);
    ExplosionHandler explosion_handler = ExplosionHandler(128, 0, 0, &camera_x, noise_map,
        Object::EXPLOSION);
    // TODO: velocity parameter useless
    ProjectileHandler projectile_handler =
        ProjectileHandler(MAX_PROJ, 8, 300 * MILLISECOND, Object::MISSILE,
        &camera_x, noise_map, &explosion_handler);

    PlayerHandler player_handler = PlayerHandler(PLAYER_SPEED, &camera_x, noise_map, &projectile_handler);

    ControlReader control_reader;

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

    /**
     * @brief Call the update physics methods for all the object handlers
     *
     */
    void update_physics();

    /**
     * @brief Create a pause menu and pause the game time;
     *
     */
    void on_pause();
};
