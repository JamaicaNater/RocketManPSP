#include "control_reader.h"

#include "globals.h"

#include <pspthreadman.h>

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

int ControlReader::read_controls(){
    int buttons_pressed = 0;
    sceCtrlReadBufferPositive(&ctrlData, 1);
    if(ctrlData.Buttons & PSP_CTRL_LEFT) {
        on_button_press_left();
        buttons_pressed++;
    } 		
    
    if(ctrlData.Buttons & PSP_CTRL_RIGHT) {
        on_button_press_right();
        buttons_pressed++;
    }
    
    if(ctrlData.Buttons & PSP_CTRL_UP) { 
        on_button_press_up();
        buttons_pressed++;
    }
    
    if(ctrlData.Buttons & PSP_CTRL_DOWN) { 
        on_button_press_down();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_CROSS) {
        on_button_press_cross();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_TRIANGLE) {
        on_button_press_triangle();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_SQUARE) {
        on_button_press_square();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_CIRCLE) {
        on_button_press_circle();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_START) { 
        on_button_press_start();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_SELECT) {
        on_button_press_select();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_RTRIGGER){
        on_button_press_r_trig();
        buttons_pressed++;
    }

    if(ctrlData.Buttons & PSP_CTRL_LTRIGGER){
        on_button_press_l_trig();
        buttons_pressed++;
    }

    return buttons_pressed;
}

void ControlReader::wait_button_release(PspCtrlButtons button){
    sceCtrlReadBufferPositive(&ctrlData, 1);
    while(ctrlData.Buttons & button){
		sceCtrlReadBufferPositive(&ctrlData, 1);
		sceKernelDelayThread(50 * MILLISECOND);
        // Wait for button to be released
	}
}