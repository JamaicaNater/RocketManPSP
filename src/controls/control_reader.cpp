#include "control_reader.h"

ControlReader::ControlReader(/* args */)
{
}

ControlReader::~ControlReader()
{
}

void ControlReader::init(){
    // For Controls
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void ControlReader::read_controls(){
    if(ctrlData.Buttons & PSP_CTRL_LEFT) {
        on_button_press_left();
    } 		
    
    if(ctrlData.Buttons & PSP_CTRL_RIGHT) {
        on_button_press_right();
    }
    
    if(ctrlData.Buttons & PSP_CTRL_UP) { 
        on_button_press_up();
    }
    
    if(ctrlData.Buttons & PSP_CTRL_DOWN) { 
        on_button_press_down();
    }

    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        on_button_press_cross();
    }

    if(ctrlData.Buttons & PSP_CTRL_TRIANGLE) {
        on_button_press_triangle();
    }

    if(ctrlData.Buttons & PSP_CTRL_SQUARE) {
        on_button_press_square();
    }

    if(ctrlData.Buttons & PSP_CTRL_CIRCLE) {
        on_button_press_circle();
    }

    if(ctrlData.Buttons & PSP_CTRL_START) { 
        on_button_press_start();
    }

    if(ctrlData.Buttons & PSP_CTRL_SELECT) {
        on_button_press_select();
    }

    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER){
        on_button_press_r_trig();
    }

    if(ctrlData.Buttons & PSP_CTRL_LTRIGGER){
        on_button_press_l_trig();
    }
}