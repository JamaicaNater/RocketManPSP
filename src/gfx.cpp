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
#include "logger/logger.h"


namespace GFX 
{

	uint32_t* draw_buffer,
			* disp_buffer;

	Image dirt("assets/dirt.bmp");
	Image sky("assets/sky.bmp");
	Image grass("assets/grass.bmp");
	Image bground("assets/background.bmp");


	bool is_transparent(unsigned int pixel) {
		return !(pixel&0xFF000000);
	}

	void init()
	{
		draw_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() );
		disp_buffer = static_cast<uint32_t *>( (uint32_t*)sceGeEdramGetAddr() + (512*480)) ;

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void load_terrain_textures() {
		load_BMP(dirt);
		load_BMP(sky);
		load_BMP(grass);
		load_BMP(bground);

		
	}


	void clear()
	{
		memset(draw_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	void swapBuffers()
	{
		uint32_t* temp = disp_buffer;
		disp_buffer = draw_buffer;
		draw_buffer = temp;

		sceKernelDcacheWritebackInvalidateAll();
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_NEXTFRAME);
	}

	void drawBMP(int x, int y, short rot, pivots pivot, char direction, const char* filename, uint32_t filter, Image &img)
	{	
		PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "Attempting to draw %s", filename);

		unsigned int * &image = img.img_matrix;
		unsigned int &width = img.width;
		unsigned int &height = img.height;
		
		// If there is not a image present generate one and store it
		if (!image) {
			load_BMP(img);			
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
		
		// Code to draw at an angle
		if (rot) {
			short playerx = x;
			short playery = y;
			// if (rot < 0) rot +=360;
			// rot %= 360;

			float rad = PI * rot /180.0f;
			float sin_theta = sin(rad);
			float cos_theta = cos(rad);
			
			unsigned short mid_x;
			unsigned short mid_y;

			switch (pivot)
			{
			case CENTER:
				mid_x = width/2;
				mid_y = height/2;
				break;
			case CENTER_LEFT:
				mid_x = 0;
				mid_y = height/2;
				break;
			case CENTER_RIGHT:
				mid_x = width;
				mid_y = height/2;
				break;
			case TOP_CENTER:
				mid_x = width/2;
				mid_y = height;
				break;
			case BOTTOM_CENTER:
				mid_x = width/2;
				mid_y = 0;
				break;
			case TOP_LEFT:
				mid_x = 0;
				mid_y = height;
				break;
			case TOP_RIGHT:
				mid_x = width;
				mid_y = height;
				break;
			\
			default:
				PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to match a pivot for %s", filename);
				break;
			}


			float x_i;
			float y_i;

			start_y = 0;
			end_y = start_y + height;
			start_x = 0;
			end_x = start_x + width;

			int location = 0;
			
			for (int y = start_y; y < end_y; y++){
				for (int x = start_x; x < end_x; x++){
					*pixel = *(image + (width*y + x));

					if (!is_transparent(*pixel)) {
						// For each pixel in the image take it and rotate it
						x_i= (x-mid_x)*cos_theta+(y-mid_y)*sin_theta;
						y_i= -(x-mid_x)*sin_theta+(y-mid_y)*cos_theta;
						//PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "", rad);

						x_i+=mid_x; 
						y_i+=mid_y; 

						if (direction == FORWARD && valid_pixel(x_i+playerx, end_y-y_i+playery, &location)) {
							//PSP_LOGGER::psp_log(PSP_LOGGER::DEBUG, "%s , %d", filename, (int)round((x_i+playerx) + SCREEN_WIDTH * (int)(end_y-y_i+playery)));
							draw_buffer[location] = *pixel;
						} else if (direction == BACKWARD && valid_pixel(end_x-x_i+playerx, end_y-y_i+playery, &location)) {
							draw_buffer[location] = *pixel;
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
							valid_pixel(x1, y1, &draw_pos);
						} else if (direction == BACKWARD) {
							valid_pixel(start_x + end_x - x1, y1, &draw_pos);
						}
						//PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "draw at %d", draw_pos);
						draw_buffer[draw_pos] = *pixel;
					}
					index++;
				}
			}
		}	

		free(pixel);
	}
	
	bool valid_pixel(int x, int y, int * location) {
		if (x < 0 || y < 0 || x > SCREEN_WIDTH_RES || y > SCREEN_HEIGHT){
			*location = SCREEN_WIDTH_RES; //Off screen
			return false;
		} else {
			*location = x + y*SCREEN_WIDTH;
			return true;
		}
	}

	void drawTerrain(unsigned char noise[], int cam_pos_x) {
		unsigned char val;
		uint32_t * target;
		int px_index;

		int img_pos_y;
		int img_pos_x;
		int img_pos_x_lagging;
		int img_pos_x_lagging_stretched;

		int y_img_pos = 0, x_img_pos = cam_pos_x;
		for(int y = 0; y <= SCREEN_HEIGHT; y++) {
			for(int x = 0; x <= SCREEN_WIDTH_RES; x++) {
				px_index = x + (SCREEN_WIDTH * y);
				val = noise[x+cam_pos_x];
				target = &draw_buffer[px_index];

				img_pos_y = (y%64)*64;
				img_pos_x = (cam_pos_x + x) % 64;
				//img_pos_x_lagging = (cam_pos_x/4 + x) % 64;
				

				if (val >= y) {
					//*target = 0xf4a903;
					if (y>=100+50) *target = 0x41b498;
					else if (y>=100) {
						*target = bground.img_matrix[((y)%50)*196 + (cam_pos_x/5 +x)%196];
					} else if (y >= 63) { //  blue sky texture
						*target = 0xba7b44;
					} else {
						img_pos_x_lagging_stretched = (cam_pos_x/6 + x)/2 % 64;
						*target = sky.img_matrix[img_pos_y+img_pos_x_lagging_stretched];
					}
				} else if (val >= y - 5) {
					//*target = 0x318c34;
					*target = grass.img_matrix[img_pos_y+img_pos_x];
				} else {
					//*target = 0x2B6F8C;
					*target = dirt.img_matrix[img_pos_y+img_pos_x];
				}
			}
		}
	}

	int do_homescreen(SceSize args, void* argp){
		unsigned int width;
		unsigned int height;

		while (1)
		{
			// TODO: use threads to break
			write_BMP(&height, &width, draw_buffer , "assets/back.bmp");
			swapBuffers();
			write_BMP(&height, &width, draw_buffer , "assets/back2.bmp");
			swapBuffers();
		}
	} 
}