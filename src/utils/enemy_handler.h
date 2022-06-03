#include "object_list.hpp"
#include "../globals.h"

class EnemyHandler
{
private:
    ObjectList enemy_list;
    unsigned int time_between_spawns = 1000*1000;
    unsigned int last_spawn = 0;

    int velocity;
    Image img;
public:
    void spawn_enemy(int x, int y, int game_time);
    void update_physics(int playerx);
    void draw(int cam_pos_x);
    EnemyHandler(int MAX_ENEMIES, int _velocity, Image _img);
    ~EnemyHandler();
};
