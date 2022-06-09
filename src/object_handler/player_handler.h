#include <pspctrl.h>

#include "object_handler.h" 
#include "projectile_handler.h"

class PlayerHandler : public ObjectHandler
{
private:
    static Object * player;

    SceCtrlData ctrlData;
    Object weapon = Object(Image("assets/player_rocket.bmp"));
    Image rocket = Image("assets/missile.bmp");

    ProjectileHandler * projectile_handler;
public:
    static Vector2d get_player_vec();
    void init();
    void read_controls();
    void update_physics();
    void draw();
    PlayerHandler(int _velocity, ProjectileHandler * _projectile_handler);
    ~PlayerHandler();
};
