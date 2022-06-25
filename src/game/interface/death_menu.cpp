#include "GUI/menu.h"

#include "game/gamestate.h"
#include "graphics/gfx.hpp"

Menu build_death_menu() {
    Menu death_menu = Menu(CENTER, 120, 90, 0xC0C0C0, 0, -20)
        .set_name("pause_menu");
    death_menu.init();

    death_menu.selected_color = 0xAAAAAA;

    death_menu.add_component(TOP_CENTER, Component("You Died")
        .set_selectable(false));

    // int edit_index = death_menu.add_component_group(CENTER, {
    //     Component("Killed: ", 0x00CC00).set_selectable(false),
    //     Component("in ", 0x00CC00).set_selectable(false)
    // }, Menu::VERTICAL_LIST, 5, 0, 20).first;

    int info_index = death_menu.add_component(BOTTOM_CENTER,
        Component("Not added").hide()).first;
    Component * info = death_menu.get_component(info_index);

    int group2 = death_menu.add_component_group(CENTER, {
        Component("Respawn", 0x00CC00).set_on_click( [info](){
            info->show();
        }),
        Component("Exit game", 0x00CC00).set_on_click([](){
            GameState::exit_game();
        })
    }, Menu::VERTICAL_LIST, 5, 0, -10).second;
    death_menu.set_selection_group(group2);

    death_menu.setup_basic_controls();

    death_menu.on_open = [](Menu * self){
        // Component * killed = self.get_component(edit_index);
        // Component * in_time = self.get_component(edit_index+1);

        // sprintf(killed->data.data.text, "Killed %d", GameState::enemies_killed);
        // sprintf(in_time->data.data.text, "in %u",
        //     curr_time - GameState::time_game_start);

        GFX::blur_screen();
        GFX::copy_buffers();
        self->update();
        self->draw_and_swap_buffers();
        while (1) {
            if (self->control_reader.read_controls()) {
                self->update();
                self->draw_and_swap_buffers();
            }
        }

        self->close();
    };

    return death_menu;
}