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
		disp_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() ) ;

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

	void drawRect(int x, int y, int w, int h, uint32_t color)
	{
		for (int y1 = y; y1 < y + h; y1++)
		{
			for (int x1 = x; x1 < x + w; x1++)
			{
				draw_buffer[x1 + (512 * y1)] = color;
			}
		}
	}

	void drawTerrain(unsigned char noise[], int cam_pos_x) {
		for(int y = 0; y <= 272; y++) {
			for(int x = 0; x <= 512; x++) {
				int px_index = x + (512 * y);
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