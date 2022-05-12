#pragma once

#include <cstdint>

#define MAX_Y 272
#define MAX_X 480

namespace GFX 
{
	void init();
	void clear(uint32_t color);
	void swapBuffers();
	void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color);
	void drawTerrain(char noise[]);
}