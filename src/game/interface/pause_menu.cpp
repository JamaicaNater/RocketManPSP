#include "pause_menu.h"

#include <pspthreadman.h>

#include "../gamestate.h"

#include "graphics/gfx.hpp"
#include "bmp/loadbmp.h"

void swap_player_image(const char * filename){
    psp_free(PlayerHandler::player->image.img_matrix);
    Image new_skin = Image(filename);
    load_BMP(new_skin);
    PlayerHandler::player->image = new_skin;
}

Menu build_pause_menu(){
    Menu pause_menu = Menu(CENTER, 120, 90, 0xC0C0C0, 0, -20);

    pause_menu.add_component(TOP_CENTER,
        Component("Game Paused", 0x00CC00)
        .set_selectable(false), 0, -5
    );

    pause_menu.add_component(CENTER,
        Component("Change skin", 0x00CC00)
        .set_selectable(false), 0, -30
    );

    int group = pause_menu.add_component_group(BOTTOM_CENTER, {
        Component("1", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player1.bmp");
        }),
        Component("2", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player2.bmp");
        }),
        Component("3", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player3.bmp");
        }),
        Component("4", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player4.bmp");
        }),
        Component("5", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player5.bmp");
        }),
        Component("6", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player6.bmp");
        }),
        Component("7", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player7.bmp");
        }),
        Component("8", 0x00CC00).set_on_click([](){
            swap_player_image("assets/player/player8.bmp");
        })
        }, Menu::GRID, 2,0,0, 2,4,false).second;
    pause_menu.set_selection_group(group);

    pause_menu.control_reader.on_button_press_start = [&pause_menu]() {
        pause_menu.control_reader.wait_button_release(PSP_CTRL_START);
        pause_time += (sceKernelGetSystemTimeLow() - GameState::status_info.start_time);
        GameState::update_status(GameState::RUNNING);
    };

    pause_menu.control_reader.on_button_press_up = [&pause_menu]() {
        pause_menu.select_next(Menu::UP);
        pause_menu.control_reader.wait_button_release(PSP_CTRL_UP);
    };

    pause_menu.control_reader.on_button_press_down = [&pause_menu]() {
        pause_menu.select_next(Menu::DOWN);
        pause_menu.control_reader.wait_button_release(PSP_CTRL_DOWN);
    };

    pause_menu.control_reader.on_button_press_left = [&pause_menu]() {
        pause_menu.select_next(Menu::LEFT);
        pause_menu.control_reader.wait_button_release(PSP_CTRL_LEFT);
    };

    pause_menu.control_reader.on_button_press_right = [&pause_menu]() {
        pause_menu.select_next(Menu::RIGHT);
        pause_menu.control_reader.wait_button_release(PSP_CTRL_RIGHT);
    };

    pause_menu.control_reader.on_button_press_cross = [&pause_menu]() {
        pause_menu.click_selection();
        pause_menu.control_reader.wait_button_release(PSP_CTRL_CROSS);
    };

    pause_menu.on_open = [](Menu &self){
        GFX::blur_screen();
        GFX::copy_buffers();
        self.update();
        self.draw_and_swap_buffers();

        self.control_reader.wait_button_release(PSP_CTRL_START);

        while (GameState::status_info.status == GameState::PAUSED)
        {
            if (self.control_reader.read_controls()){
                self.update();
                self.draw_and_swap_buffers();
            }
        }

        self.close();
    };

    return pause_menu;
}
