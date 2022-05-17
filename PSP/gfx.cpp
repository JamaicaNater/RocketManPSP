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

	float sin_table[360];
	float cos_table[360];

	void init()
	{
		draw_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() );
		disp_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() + (512*480*4)) ;

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void populate_trig_tables(){
		for (int i = 0; i < 360; i++) sin_table[i] = sin((i * (PI / 180)));
		for (int i = 0; i < 360; i++) cos_table[i] = cos((i * (PI / 180)));
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

	
	void drawPNG(int x, int y, short rot, char direction, char* filename, uint32_t filter, unsigned char * &image)
	{
		unsigned error;
		//unsigned char* image = 0;
		unsigned width, height;

		if (!image) {
			error = lodepng_decode32_file(&image, &width, &height, filename);
			if(error) pspDebugScreenPrintf("error %u: %s %u Avaliable\n", error, lodepng_error_text(error), sceKernelTotalFreeMemSize());

			// for (int i = 0 i < width*height; i++) {
			// 	RGB_to_BGR(&image[i], &image[i])
			// }
		} 
		

		//TODO create list of images in memory to be freed in the case of error 83
		// TODO pass the entire projectile object in
		// TODO set the worm image in case of null
		


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

		if (rot) {
			unsigned short playerx = x;
			unsigned short playery = y;
			if (rot < 0) rot +=360;
			rot %= 360;
			
			unsigned short mid_x = width/2;
			unsigned short mid_y = height/2;
			float x_i;
			float y_i;

			start_y = 0;
			end_y = start_y + height;
			start_x = 0;
			end_x = start_x + width;

			unsigned short y_pos_sub_mid = start_x-mid_x;
			unsigned short x_pos_sub_mid = start_y-mid_y;
			unsigned short draw_pos;
			
			for (int y = start_y; y < end_y; y++){
				for (int x = start_x; x < end_x; x++){
					if (RGB_to_BGR(image + 4*(width*y + x), pixel)) {
						x_i= (x-mid_x)*cos_table[rot]+(y-mid_y)*sin_table[rot];
						y_i= -(x-mid_x)*sin_table[rot]+(y-mid_y)*cos_table[rot];

						x_i=round(x_i)+mid_x; 
						y_i=round(y_i)+mid_y; 

						if (x_i+playerx>=0 && y_i+playery>=0) {
							draw_buffer[static_cast<int>((x_i + playerx) + SCREEN_WIDTH * (y_i+playery))] = *pixel;
						}
					}
				}
			}
		} else {
			start_y = y;
			end_y = y + height;
			start_x = x;
			end_x = x + width;

			int draw_pos;

			for (int y1 = start_y; y1 < end_y; y1++)
			{
				for (int x1 = start_x; x1 < end_x; x1++)
				{
					if (RGB_to_BGR((image+index), pixel)){
						
						
						switch (direction)
						{
						case FORWARD:
							draw_pos = (x1) + SCREEN_WIDTH * y1;
							break;

						case BACKWARD:
							draw_pos = (start_x + end_x - x1) + SCREEN_WIDTH * y1;
							break;
						
						default:
							break;
						}


						draw_buffer[draw_pos] = *pixel;
					}
					index+=4;
				}
			}
		}
		

		free(pixel);
		//free(image);
	}

	int RGB_to_BGR(unsigned char* RGB, uint32_t * BGR){
				unsigned char red = *(RGB);
				unsigned char green = *(RGB+1);
				unsigned char blue = *(RGB+2);
				unsigned char alpha = *(RGB+3);

				*BGR =alpha<<24 | blue<<16 | green<<8 | red;
				return alpha;
	}

	bool valid_pixel(int x, int y, int * location) {
		if (x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return false;

		*location = x + y*512;
		return true;
	}

	void drawTerrain(unsigned char noise[], int cam_pos_x) {
		unsigned char val;
		uint32_t * target;
		int px_index;
		for(int y = 0; y <= SCREEN_HEIGHT; y++) {
			for(int x = 0; x <= SCREEN_WIDTH; x++) {
				px_index = x + (SCREEN_WIDTH * y);
				val = noise[x+cam_pos_x];
				target = &draw_buffer[px_index];

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