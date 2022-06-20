#include "game/interface/main_menu.h"

#include "GUI/menu.h"

#include "bmp/loadbmp.h"
#include "game/gamestate.h"

Menu build_main_menu() {
    Menu main_menu = Menu(CENTER, SCREEN_HEIGHT, SCREEN_WIDTH_RES, 0xC0C0C0, 0, 0);
    Image menu_image = Image("assets/game/menu.bmp");
    load_BMP(menu_image);

    main_menu.add_component(CENTER, Component(menu_image));

    int group1 = main_menu.add_component_group(CENTER,{
        Component("Arcade Mode").set_on_click([](){
            GameState::in_menu = false;
        }),
        Component("Story Mode")
    }, Menu::HORIZONTAL_LIST, 5).second;
    main_menu.set_selection_group(group1);

    main_menu.setup_basic_controls();

    main_menu.update();
    main_menu.draw_and_swap_buffers();

    main_menu.on_open = [](Menu &self){
         while (GameState::in_menu) {
            if (self.control_reader.read_controls()) {
                self.update();
                self.draw_and_swap_buffers();
            }
        }
        self.close();
    };

    return main_menu;
}