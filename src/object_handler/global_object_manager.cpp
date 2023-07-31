#include "gfx.hpp"
#include "game_globals.h"
#include "object/object_list.h"
#include "global_object_manager.h"

namespace ObjectManager {
    void draw_health_bars(){
        Object ** glob_objects = ObjectList::get_global_object_list()->get_list();
        for (int i = 0; i < ObjectList::get_global_object_list()->MAX_SIZE; i++) {
            if (!glob_objects[i]) continue;
            if (glob_objects[i]->type != Object::ENEMY) continue;
            if (glob_objects[i]->off_screen(camera_x)) continue;


            GFX::draw_progress_bar(glob_objects[i]->get_draw_x(camera_x),
                glob_objects[i]->get_draw_y() - 5, 3, 30,
                glob_objects[i]->health, glob_objects[i]->max_health, 0xFF00FF00, 0xFF0000FF);
            }
    }
}

