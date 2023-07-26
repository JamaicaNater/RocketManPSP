#pragma once

#include <pspctrl.h>
#include "game/game_globals.h"
#include "object/object_handler.h"
#include "projectile_handler.h"

#include "controls/control_reader.h"

#include "game/interface/pause_menu.h"

class PlayerHandler : public ObjectHandler
{
public:
    static Object * player;
    static Object get_player_val();

    /**
     * @brief Insert player and weapon into object list set the positions an
     * setup controls for reading
     *
     */
    void init() override;

    /**
     * @brief Reads in the player controls
     *
     */
    void read_controls();

    /**
     * @brief Set the up the events that will happen when each button was
     * pressed
     *
     */
    void set_up_controls();

    /**
     * @brief updates the physics and sets the weapon positions
     *
     */
    virtual void update_physics() override;

    /**
     * @brief When the player goes of screen teleport the player back on the
     * screen
     *
     * @param obj
     */
    virtual void on_off_screen(Object * obj) override;

    /**
     * @brief when the player dies do the death screen
     *
     * @param obj
     */
    virtual void on_zero_health(Object * obj) override;

    PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler);
    ~PlayerHandler();
private:
    ControlReader player_control_reader;
    Object * weapon = new Object(Image("assets/player/playerrocket.bmp"));
    Image rocket = Image("assets/misc/missile.bmp");

    ProjectileHandler * projectile_handler;
};
