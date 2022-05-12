#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <math.h>
#include <pspdebug.h>


namespace GFX 
{

	uint32_t* draw_buffer,
			* disp_buffer;
	void init()
	{
		draw_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() );
		disp_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() ) + (512 * 272 * 4);

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void clear(uint32_t color)
	{
		for (int i = 0; i < 512 * 272; i++)
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

	void drawRect( unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t color)
	{
		if (x+w > 480)
			x = 480-w;

		if (y+h > 272)
			y = 272-h;

		if ((x + w) > 480)
			w = 480 - x;

		if ((y + h) > 272)
			h = 272 - y;

		int off = x + (512 * y);

		int size = w * h;
		int arr[size];



		for (int y1 = 0; y1 < h; y1++)
		{
			for (int x1 = 0; x1 < w; x1++)
			{
				draw_buffer[off + x1 + (512 * y1)] = color;
			}
		}

		//delete arr;

	}

	void drawTerrain(char noise[]) {
		//for (int i = 0; i < 512*65; i++) draw_buffer[i] = 0xFFFFFFF;


		int num = pow(16,2);
		for(float y = 0; y <= 272; y++) {
			for(float x = 0; x <= 512; x++) {
				float val = noise[static_cast<int>( x + (512 * y) )];
				uint32_t * target = &draw_buffer[static_cast<int>( x + (512 * y) )];
				//draw_buffer[static_cast<int>( x + (512 * y) )] = 0x00000000 + num * static_cast<int>(noise[static_cast<int>( x + (512 * y) )]);
				//(static_cast<int>(x) % 30 == 0) pspDebugScreenPrintf("Ox%08X  ", 0x00000000 + num * static_cast<int>(noise[static_cast<int>( x + (512 * y) )]));

				if (val < 60) { *target = 0xf4a903; }
				else if (val < 200 ) {*target = 0x318c34; }
				else if (val < 220) {*target = 0x697681; }
				else if (val < 240) {*target = 0xFFFaFaFF;}
				else {*target = 0xFFaf7e0;}
			}
		}

	}
}