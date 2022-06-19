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

    title.add_component_group(CENTER, {
        Component("Play Game", 0x22B681)
        .set_on_click([](){
            GameState::in_title = false;
        }),

        Component("Exit Game", 0x1125FF)
        .set_on_click([](){
            GameState::exit_game();
        })
        }, Menu::HORIZONTAL_LIST, 10, 0, -40);

    title.add_component(BOTTOM_LEFT, 
    Component("Copyright 2022 Francis Williams", 0xFFFFFF)
    .set_selectable(false)
    );

    title.control_reader.on_button_press_up = [&title]() {
        title.select_next(Menu::UP);
        title.control_reader.wait_button_release(PSP_CTRL_UP);
    };

    title.control_reader.on_button_press_down = [&title]() {
        title.select_next(Menu::DOWN);
        title.control_reader.wait_button_release(PSP_CTRL_DOWN);
    };

    title.control_reader.on_button_press_left = [&title]() {
        title.select_next(Menu::LEFT);
        title.control_reader.wait_button_release(PSP_CTRL_LEFT);
    };

    title.control_reader.on_button_press_right = [&title]() {
        title.select_next(Menu::RIGHT);
        title.control_reader.wait_button_release(PSP_CTRL_RIGHT);
    };

    title.control_reader.on_button_press_cross = [&title]() {
        title.click_selection();
        title.control_reader.wait_button_release(PSP_CTRL_CROSS);
    };
    
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
    };

    return title;
}