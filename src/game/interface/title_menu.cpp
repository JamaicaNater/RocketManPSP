#include "title_menu.h"

#include "bmp/loadbmp.h"
#include "GUI/menu.h"
#include "game/gamestate.h"

Menu build_title_menu() {
    Menu title = Menu(CENTER, SCREEN_HEIGHT, SCREEN_WIDTH_RES, 0xC0C0C0, 0, 0);
    title.selected_color = 0x002000;

    Image title_img =  Image("assets/game/titlescreen.bmp");
    load_BMP(title_img);

    title.add_component(CENTER, Component(title_img).set_selectable(false) );

    int group = title.add_component_group(CENTER, {
        Component("Play Game", 0x22B681)
        .set_on_click([](){
            GameState::in_title = false;
        }),

        Component("Exit Game", 0x1125FF)
        .set_on_click([](){
            GameState::exit_game();
        })
        }, Menu::HORIZONTAL_LIST, 10, 0, -40) .second;
    title.set_selection_group(group);

    title.add_component(BOTTOM_LEFT,
    Component("Copyright 2022 Francis Williams", 0xFFFFFF)
    .set_selectable(false)
    );

    title.setup_basic_controls();

    title.on_open = [](Menu &self){
        self.update();
        self.draw_and_swap_buffers();
        while (GameState::in_title)
        {
            if (self.control_reader.read_controls()){
                self.update();
                self.draw_and_swap_buffers();
            }
        }

        self.close();
    };

    return title;
}