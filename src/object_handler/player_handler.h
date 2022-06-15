#pragma once

#include <pspctrl.h>

#include "object_handler.h" 
#include "projectile_handler.h"
#include "../controls/control_reader.h"

class PlayerHandler : public ObjectHandler
{
private:
    static Object * player;

    ControlReader player_control_reader;
    Object * weapon = new Object(Image("assets/player_rocket.bmp"));
    Image rocket = Image("assets/missile.bmp");

    ProjectileHandler * projectile_handler;
public:
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

    void set_up_controls();
    
    /**
     * @brief updates the physics and sets the weapon positions
     * 
     */
    void update_physics() override;
    
    /**
     * @brief When the player goes of screen teleport the player back on the 
     * screen
     * 
     * @param obj 
     */
    void on_off_screen(Object * obj) override;
    
    PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler);
    ~PlayerHandler();
};
