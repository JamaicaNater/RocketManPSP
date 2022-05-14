#pragma once

#include <cstdint>

#define MAX_Y 272
#define MAX_X 480

namespace GFX 
{
	void init();
	void clear(uint32_t color);
	void swapBuffers();
	void drawRect(int x, int y, int w, int h, uint32_t color);
	void drawTerrain(unsigned char noise[], int cam_pos_x);
}