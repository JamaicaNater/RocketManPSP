#include <functional>
#include <pspctrl.h>

class ControlReader
{
public:
    SceCtrlData ctrlData;

    std::function<void()> on_button_press_cross;
    std::function<void()> on_button_press_triangle;
    std::function<void()> on_button_press_square;
    std::function<void()> on_button_press_circle;

    std::function<void()> on_button_press_up;
    std::function<void()> on_button_press_down;
    std::function<void()> on_button_press_left;
    std::function<void()> on_button_press_right;

    std::function<void()> on_button_press_l_trig;
    std::function<void()> on_button_press_r_trig;

    std::function<void()> on_button_press_start;
    std::function<void()> on_button_press_select;

    ControlReader(/* args */);
    ~ControlReader();
    void init();
    void read_controls();
};
