#include "object_list.hpp"

class EnemyHandler
{
private:
    ObjectList enemy_list;
    int time_between_spawns = 4*1000*1000;
    int last_spawn = 0;
    
    unsigned char * NOISE_MAP;
    int MAP_SIZE;
public:
    Image enemy_img = Image("assets/enemy.bmp");
    void spawn_enemy(int x, int y, int game_time);
    void update_physics(int playerx);
    void draw(int cam_pos_x);
    EnemyHandler(int MAX_ENEMIES, int MAP_SIZE, unsigned char * NOISE_MAP);
    ~EnemyHandler();
};
