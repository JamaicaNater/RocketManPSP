#include "title_menu.h"
#include "../../bmp/loadbmp.h"

Menu build_title_menu() {
    Menu title = Menu(CENTER, SCREEN_HEIGHT, SCREEN_WIDTH_RES, 0xC0C0C0, 0, 0);
    title.selected_color = 0x003000;

    Image title_img =  Image("assets/titlescreen.bmp");
    load_BMP(title_img);

    title.add_component(CENTER, Component(title_img).set_selectable(false) );

    title.add_component_group(CENTER, {
        Component("Play Game", 0x22B681),
        Component("Exit Game", 0x1125FF)
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
    
    title.update();  
    GFX::simple_drawBMP(title.x, title.y, title.gui);
    GFX::swapBuffers();
    while (1)
    {   
        if (title.control_reader.read_controls()){
            title.update();  
            GFX::simple_drawBMP(title.x, title.y, title.gui);
            GFX::swapBuffers();
        } 
    }

    return title;
}