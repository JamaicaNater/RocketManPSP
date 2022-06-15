#pragma once

#include <functional>
#include <pspctrl.h>

class ControlReader
{
private:
    SceCtrlData ctrlData; 
    std::function<void()> empty = [](){};  

public:
    std::function<void()> on_button_press_cross = empty;
    std::function<void()> on_button_press_triangle = empty;
    std::function<void()> on_button_press_square = empty;
    std::function<void()> on_button_press_circle = empty;

    std::function<void()> on_button_press_up = empty;
    std::function<void()> on_button_press_down = empty;
    std::function<void()> on_button_press_left = empty;
    std::function<void()> on_button_press_right = empty;

    std::function<void()> on_button_press_l_trig = empty;
    std::function<void()> on_button_press_r_trig = empty;

    std::function<void()> on_button_press_start = empty;
    std::function<void()> on_button_press_select = empty;

    ControlReader(/* args */);
    ~ControlReader();
    void init();
    void read_controls();
    void wait_button_release(PspCtrlButtons button);
};
