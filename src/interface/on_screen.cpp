#include "interface/on_screen.h"

#include "globals.h"

#include "gamestate.h"

#include "GUI/menu.h"

int counter_index;
int prev_kills = 0;

int clock_index;
int prev_time;

Menu build_kill_counter() {
    Menu kill_counter = Menu(TOP_RIGHT, 30, 60, 0x3440EB);
    int comp_icon = kill_counter.add_component(CENTER_LEFT,
        Component(Image("assets/enemy/enemy.bmp").load().resize(.5))).first;

    int padding_x = kill_counter.get_component(comp_icon)->width;

    counter_index = kill_counter.add_component(CENTER_LEFT, Component("x 0"),
        padding_x).first;

    return kill_counter;
}

void update_kills(Menu& kill_counter) {
    if (prev_kills == GameState::enemies_killed || prev_kills > 99) return;
    prev_kills = GameState::enemies_killed;

    log(DEBUG, "updating kills to %d", prev_kills);

    Component * counter = kill_counter.get_component(counter_index);
    sprintf(counter->data.data.text, "x %d", GameState::enemies_killed);
}

Menu build_game_time_icon() {
    Menu game_time_icon = Menu(TOP_CENTER, 20, 30, 0xC0C0C0);

    clock_index = game_time_icon.add_component(CENTER,
        Component("0s")).first;

    return game_time_icon;
}

void update_game_time_icon(Menu & clock) {
    log(DEBUG, "updating kills to %d", prev_kills);

    Component * clock_comp = clock.get_component(clock_index);
    sprintf(clock_comp->data.data.text, "%ds",
        (curr_time - GameState::time_game_start)/SECOND);
}