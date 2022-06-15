#include "pause_menu.h"

#include <pspthreadman.h>

#include "../../gamestate.h"

Menu build_pause_menu(){
    Menu pause_menu = Menu(CENTER, 120, 90, 0xC0C0C0, 0, -20);
    
    pause_menu.add_component(BOTTOM_CENTER, 
        Component(10,20, Component::Rectangle, 0x00FF00),
        0, 20);
    pause_menu.add_component(CENTER, Component("Game Paused"));

    
    pause_menu.control_reader.on_button_press_start = [&pause_menu](){       
        pause_menu.control_reader.wait_button_release(PSP_CTRL_START);  
        pause_time += (sceKernelGetSystemTimeLow() - GameState::status_info.start_time);
        GameState::update_status(GameState::RUNNING);
    };
    
    pause_menu.on_open = [](Menu &self){
        GFX::blur_screen();
        self.update();
        GFX::simple_drawBMP(self.x, self.y, self.gui);
        GFX::swapBuffers();

        self.control_reader.wait_button_release(PSP_CTRL_START);   
        while (GameState::status_info.status == GameState::PAUSED)
        {   
            self.control_reader.read_controls();
        }
    };

    return pause_menu;
}