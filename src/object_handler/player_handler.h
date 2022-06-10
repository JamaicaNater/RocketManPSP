#pragma once

#include <pspctrl.h>

#include "object_handler.h" 
#include "projectile_handler.h"

class PlayerHandler : public ObjectHandler
{
private:
    static Object * player;

    SceCtrlData ctrlData;
    Object * weapon = new Object(Image("assets/player_rocket.bmp"));
    Image rocket = Image("assets/missile.bmp");

    ProjectileHandler * projectile_handler;
public:
    static Object get_player_val();
    void init() override;
    void read_controls();
    void update_physics() override;
    void on_off_screen(Object * obj) override;
    PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler);
    ~PlayerHandler();
};
