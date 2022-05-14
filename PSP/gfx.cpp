#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <math.h>
#include <pspdebug.h>
#include "png/lodepng.h"

namespace GFX 
{

	uint32_t* draw_buffer,
			* disp_buffer;
	void init()
	{
		draw_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() );
		disp_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() + (512*480*4)) ;

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void clear(uint32_t color)
	{
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
			draw_buffer[i] = color;
	}

	void swapBuffers()
	{
		uint32_t* temp = disp_buffer;
		disp_buffer = draw_buffer;
		draw_buffer = temp;

		sceKernelDcacheWritebackInvalidateAll();
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_NEXTFRAME);
	}

	void drawRect(int x, int y, int w, int h, uint32_t color)
	{
		for (int y1 = y; y1 < y + h; y1++)
		{
			for (int x1 = x; x1 < x + w; x1++)
			{
				draw_buffer[x1 + (SCREEN_WIDTH * y1)] = color;
			}
		}
	}

	void drawPNG(int x, int y, char* filename)
	{
		unsigned error;
		unsigned char* image = 0;
		unsigned width, height;

		error = lodepng_decode32_file(&image, &width, &height, filename);
		if(error) pspDebugScreenPrintf("error %u: %s\n", error, lodepng_error_text(error));

		// /*use image here*/
		// printf("%s", image);
		y-=height;
		x-=width/2;
		int index = 0;
		
		int start_y = y;
		int end_y = y + height;
		int start_x = x;
		int end_x = x + width;

		for (int y1 = start_y; y1 < end_y; y1++)
		{
			for (int x1 = start_x; x1 < end_x; x1++)
			{
				unsigned char red = *(image+index);
				unsigned char green = *(image+index+1);
				unsigned char blue = *(image+index+2);
				unsigned char alpha = *(image+index+3);

				if ((int)alpha > 0){
					uint32_t pixel = blue<<16 | green<<8 | red;
					draw_buffer[x1 + (SCREEN_WIDTH * y1)] = pixel;
				}
				index+=4;
			}
		}
		free(image);
	}

	void drawTerrain(unsigned char noise[], int cam_pos_x) {
		for(int y = 0; y <= SCREEN_HEIGHT; y++) {
			for(int x = 0; x <= SCREEN_WIDTH; x++) {
				int px_index = x + (SCREEN_WIDTH * y);
				float val = noise[x+cam_pos_x];
				uint32_t * target = &draw_buffer[px_index];

				if (val >= y) {
					*target = 0xf4a903;
				} else if (val >= y - 5) {
					*target = 0x318c34;			
				} else {
					*target = 0x2B6F8C;
				}
				
			}
		}

	}
}