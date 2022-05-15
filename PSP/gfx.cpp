#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <math.h>
#include <pspdebug.h>
#include <pspsysmem.h>

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

	
	void drawPNG(int x, int y, short rot, char flip, char* filename, uint32_t filter)
	{
		unsigned error;
		unsigned char* image = 0;
		unsigned width, height;

		error = lodepng_decode32_file(&image, &width, &height, filename);
		if(error) pspDebugScreenPrintf("error %u: %s %u Avaliable\n", error, lodepng_error_text(error), sceKernelTotalFreeMemSize());

		// /*use image here*/
		// printf("%s", image);
		//y = 272 - y;
		y-=height;
		x-=width/2;
		int index = 0;
		

		uint32_t * pixel = new(uint32_t);
		int start_y = y;
		int end_y = y + height;
		int start_x = x;
		int end_x = x + width;

		if (!rot) {
			start_y = y;
			end_y = y + height;
			start_x = x;
			end_x = x + width;

			for (int y1 = start_y; y1 < end_y; y1++)
			{
				for (int x1 = start_x; x1 < end_x; x1++)
				{
					if (RGB_to_BGR((image+index), pixel)){
						int index;
						
						switch (flip)
						{
						case FORWARD:
							index = (x1) + SCREEN_WIDTH * y1;
							break;

						case BACKWARD:
							index = (start_x + end_x - x1) + SCREEN_WIDTH * y1;
							break;
						
						default:
							break;
						}


						draw_buffer[index] = *pixel;
					}
					index+=4;
				}
			}
		} else {
				
				//First we will convert the degrees into radians
				double pi = 3.14159265359;
				float rads = (rot * (pi / 180));

				//Finding the center point of rotated (or original) image.
				int r_height = height;
				int r_width  = width;

				int mid_x = width/2;
				int mid_y = height/2;
				int x_i;
				int y_i;

				start_y = 0;
				end_y = y + height*3;
				start_x = 0;
				end_x = x + width*3;
				
				for (int y = start_y; y < end_y; y++){
					for (int x = start_y; x < end_y; x++){
						x_i= (x-mid_x)*cos(rads)+(y-mid_y)*sin(rads);
						y_i= -(x-mid_x)*sin(rads)+(y-mid_y)*cos(rads);

						x_i=round(x_i)+mid_x; 
						y_i=round(y_i)+mid_y; 

						if (x_i>=0 && y_i>=0 && x_i<r_width && y_i<r_height && RGB_to_BGR((image + 4*(width*y_i + x_i) ), pixel)) {
							draw_buffer[(x) + SCREEN_WIDTH * y] = *pixel;
						}
						index +=4;
					}
				}
		}
		

		free(pixel);
		free(image);
	}

	int RGB_to_BGR(unsigned char* RGB, uint32_t * BGR){
				unsigned char red = *(RGB);
				unsigned char green = *(RGB+1);
				unsigned char blue = *(RGB+2);
				unsigned char alpha = *(RGB+3);

				*BGR = blue<<16 | green<<8 | red;
				return alpha;
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