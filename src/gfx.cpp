#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <math.h>
#include <pspdebug.h>
#include <pspsysmem.h>
#include <pspkernel.h>
#include <pspctrl.h>

#include "bmp/loadBMP.h"
#include "utils.h"

namespace GFX 
{

	uint32_t* draw_buffer,
			* disp_buffer;

	float sin_table[360];
	float cos_table[360];

	unsigned int format_pixel(unsigned int data)
	{
		// Used to Format Pixels from ARGB to ABGR for the PSP display
		return (data&0xFF000000) | ((data&0xFF000000)>>24 | (data&0x00FF0000)>>8 | (data&0x0000FF00)<<8 | (data&0x000000FF)<<24)>>8;
	}

	bool is_transparent(unsigned int pixel) {
		// Masks the Alpha channel and returns false if t has a value grater than 0
		return !(pixel&0xFF000000);
	}

	// unsigned int swap_endian(unsigned int data)
	// {
	// 	return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
	// }

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


	/**
	 * @brief 
	 * 
	 * @param x -  x position to draw at
	 * @param y -  y position to draw at
	 * @param rot - angle of the object
	 * @param direction - direction of the object
	 * @param filename 
	 * @param filter - TBD
	 * @param image 
	 */
	void drawBMP(int x, int y, short rot, char direction, const char* filename, uint32_t filter, unsigned int * &image)
	{
		unsigned int width;
		unsigned int height;
		
		// If there is not a image present generate one and store it
		if (!image) {		
			unsigned int * temp_img;
			load_BMP(&height, &width,temp_img , filename);

			image = new unsigned int[width*height];
			for (int i = 0; i < width*height; i++) {
				image[i] = format_pixel(temp_img[i]);
			}
		
			free(temp_img);
		} 
		

		//TODO create list of images in memory to be freed in the case of no more memory		

		//TODO: change behaivor of function such that the program stticty draw at the passed in x and y positions
		y-=height;
		x-=width/2;
		int index = 0;
		
		uint32_t * pixel = new(uint32_t);
		int start_y = y;
		int end_y = y + height;
		int start_x = x;
		int end_x = x + width;
		
		// Code t draw at an angle
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
					*pixel = *(image + (width*y + x));
					if (!is_transparent(*pixel)) {
						// For ieach pixel in the image take it and rotate it
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
			start_y = y + height;
			end_y = y;
			start_x = x;
			end_x = x + width;

			int draw_pos;

			index = 0;
			for (int y1 = start_y; y1 > end_y; y1--)
			{
				for (int x1 = start_x; x1 < end_x; x1++)
				{
					*pixel = *(image+index);
					if (!is_transparent(*pixel)){
						
						if(direction == FORWARD) {
							draw_pos = (x1) + SCREEN_WIDTH * y1;
						} else {
							draw_pos = (start_x + end_x - x1) + SCREEN_WIDTH * y1;
						}

						draw_buffer[draw_pos] = *pixel;
					}
					index++;
				}
			}
		}	

		free(pixel);
	}
	
	/**
	 * @brief Performs bounds checking to make sure the area we attempt to write to can be written to
	 * 
	 * @param x 
	 * @param y 
	 * @param location x and y converted for use in 1d array
	 * @return true valid position
	 * @return false invalid position
	 */
	bool valid_pixel(int x, int y, int * location) {
		if (x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return false;

		*location = x + y*512;
		return true;
	}

	/**
	 * @brief Darws the backround terrain of the worm
	 * 
	 * @param noise Psudorandom perlin noise 
	 * @param cam_pos_x Position of the camera
	 */
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

	void do_homescreen(){
		unsigned int width;
		unsigned int height;

		unsigned int delta = 1000*1000*15, end_time;\
		sceCtrlSetSamplingCycle(0);
		sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
		SceCtrlData ctrlData;
		end_time = sceKernelGetSystemTimeLow() + delta;

		while (1)
		{
			// TODO: use threads to break
			sceCtrlReadBufferPositive(&ctrlData, 1);
			if(ctrlData.Buttons & PSP_CTRL_START) break;
			write_BMP(&height, &width, draw_buffer , "assets/back.bmp");
			swapBuffers();
			if(ctrlData.Buttons & PSP_CTRL_START) break;
			write_BMP(&height, &width, draw_buffer , "assets/back2.bmp");
			swapBuffers();
			if(ctrlData.Buttons & PSP_CTRL_START) break;
		}
		

	}
}