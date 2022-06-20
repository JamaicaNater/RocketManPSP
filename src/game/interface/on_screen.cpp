#include "game/interface/on_screen.h"

#include "game/gamestate.h"

#include "GUI/menu.h"

int counter_index;
int prev_kills = 0;

Menu build_kill_counter() {
    Menu kill_counter = Menu(TOP_RIGHT, 30, 60, 0x3440EB);
    int comp_icon = kill_counter.add_component(CENTER_LEFT,
        Component(Image("assets/enemy/enemy.bmp").load().resize(.5))).first;

    int padding_x = kill_counter.get_component(comp_icon)->width;

    counter_index = kill_counter.add_component(CENTER_LEFT, Component("x 10"),
        padding_x).first;

    kill_counter.log_comp_data();

    return kill_counter;
}

void update_kills(Menu& kill_counter) {
    if (prev_kills == GameState::enemies_killed || prev_kills > 99) return;
    prev_kills = GameState::enemies_killed;

    log(DEBUG, "updating kills to %d", prev_kills);

    Component * counter = kill_counter.get_component(counter_index);
    sprintf(counter->data.data.text, "x %d", GameState::enemies_killed);
}