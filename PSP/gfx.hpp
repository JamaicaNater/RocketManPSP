#pragma once

#include <cstdint>

#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH_RES 480
#define SCREEN_WIDTH 512
#define PI 3.14159265359

enum flips{FORWARD, BACKWARD};

namespace GFX 
{
	void init();
	void clear(uint32_t color);
	void swapBuffers();
	void drawRect(int x, int y, int w, int h, uint32_t color);
	void drawTerrain(unsigned char noise[], int cam_pos_x);
	void drawPNG(int, int, short, char, char*, uint32_t , unsigned char* &);
	int RGB_to_BGR(unsigned char* RGB, uint32_t * BGR);
	void populate_trig_tables();
}