#include "gfx.hpp"
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <math.h>
#include <pspdebug.h>
#include <pspsysmem.h>
#include <pspkernel.h>
#include <pspctrl.h>

#include "globals.h"

#include "bmp/loadBMP.h"
#include "logger/logger.h"
#include "image/image_utils.h"


namespace GFX 
{

	uint32_t* draw_buffer,
			* disp_buffer;

	Image dirt("assets/terrain/dirt.bmp");
	Image sky("assets/terrain/sky.bmp");
	Image grass("assets/terrain/grass.bmp");
	Image bground("assets/terrain/background.bmp");


	bool is_transparent(unsigned int pixel) {
		return !(pixel&0xFF000000);
	}

	void init()
	{
		draw_buffer = static_cast<uint32_t *>( sceGeEdramGetAddr() );
		disp_buffer = static_cast<uint32_t *>( (uint32_t*)sceGeEdramGetAddr() + 
			(SCREEN_WIDTH*480));

		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void load_terrain_textures() {
		load_BMP(dirt);
		load_BMP(sky);
		load_BMP(grass);
		load_BMP(bground);		
	}

	void blur_screen(){
		Image blurred_screen = Image(SCREEN_HEIGHT,SCREEN_WIDTH, draw_buffer, "screen");
		blurred_screen.blur();
	}

	void tint_screen(uint32_t tint_factor){
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) draw_buffer[i] = tint_pixel(draw_buffer[i], tint_factor);
	}

	void clear()
	{
		//memset(draw_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	void copy_buffers(){
		memcpy(disp_buffer, draw_buffer, sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	void swapBuffers()
	{
		uint32_t* temp = disp_buffer;
		disp_buffer = draw_buffer;
		draw_buffer = temp;

		sceKernelDcacheWritebackInvalidateAll();
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 
			PSP_DISPLAY_SETBUF_NEXTFRAME);
	}

	void draw_progress_bar(int x, int y, int height, int width, int val, 
		int max, uint32_t val_color, uint32_t rem_color
	){
		int val_width = (val * 1.0f / max) * width;

		if (y < 0) y = 0;
		if (y > SCREEN_HEIGHT) return;

		if(x < 0) x = 0;
		if(x > SCREEN_WIDTH_RES) return;

		if (y + height > SCREEN_HEIGHT) height = SCREEN_HEIGHT - y;
		if (x + width > SCREEN_WIDTH_RES) width = SCREEN_WIDTH_RES - x;

		for (int y_i = y+height; y_i > y; y_i--){
			for (int x_i = x; x_i < x+width; x_i++) {
				if (x_i < x+val_width) draw_buffer[y_i*SCREEN_WIDTH+x_i] = val_color;
				else draw_buffer[y_i*SCREEN_WIDTH+x_i] = rem_color;
			}
		}
	}

	void simple_drawBMP(int x, int y, Image &img, bool disp_buf) {
		uint32_t * buffer = (disp_buf) ? disp_buffer : draw_buffer;

		if (!img.img_matrix) {
			load_BMP(img);
		}

		int index = 0;
		int start_y = y + img.height;
		int end_y = y;

		for (int y1 = start_y; y1 > end_y; y1--)
		{
			memcpy(buffer + (y1*SCREEN_WIDTH) + x, img.img_matrix+index, 
				img.width * sizeof(uint32_t));
			index += img.width;
		}
	}

	void drawBMP(int x, int y, short rot, Position pivot, Flips direction, 
		uint32_t filter, Image &img
	){	
		unsigned int * &image = img.img_matrix;
		unsigned int &width = img.width;
		unsigned int &height = img.height;
		// If there is not a image present generate one and store it
		if (!image) {
			log(DEBUG, "call loadBMP on %s", img.filename);
			load_BMP(img);			
		} 

		//TODO create list of images in memory to be freed in the case of 
		//no more memory		
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
			
			unsigned short mid_x = 0;
			unsigned short mid_y = 0;

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
				log(CRITICAL, "Failed to match a pivot for %s", img.filename);
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

						x_i+=mid_x; 
						y_i+=mid_y; 

						if (direction == FORWARD && valid_pixel(x_i+playerx, end_y-y_i+playery, &location)) {
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
						} else{
							valid_pixel(start_x + end_x - x1, y1, &draw_pos);
						}
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

	void drawTerrain() {
		unsigned char val;
		uint32_t * target;
		int px_index;
		int img_pos_y;
		int img_pos_x;

		int y_i;
		int y_f;
		int img_start_pos;
		int first_chunk_size;
		int chunk_size;
		int last_chunk_size;

		//TODO: increase readability

		//Draw sky bmp
		y_i = 0;
		y_f = y_i + sky.height;
		img_start_pos = (camera_x/6) % sky.width;
		first_chunk_size = sky.width-img_start_pos;
		chunk_size = sky.width;
		last_chunk_size = SCREEN_WIDTH_RES - (first_chunk_size+384);
		last_chunk_size *= (last_chunk_size>0);
		for(int y = y_i; y < y_f; y++){
			memcpy(draw_buffer + (SCREEN_WIDTH*y), sky.img_matrix+(y)*sky.width+img_start_pos, first_chunk_size * sizeof(unsigned int));
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size), sky.img_matrix+(y)*sky.width, chunk_size * sizeof(unsigned int));
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size+128), sky.img_matrix+(y)*sky.width, chunk_size * sizeof(unsigned int));
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size+256), sky.img_matrix+(y)*sky.width, chunk_size * sizeof(unsigned int));
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size+384), sky.img_matrix+(y)*sky.width, last_chunk_size * sizeof(unsigned int));
		}

		//Draw Blue sky
		for (int y = 64; y < 100; y++){
			for (int x = 0; x < SCREEN_WIDTH_RES; x++) draw_buffer[y*SCREEN_WIDTH + x] = 0xba7b44;
		}

		//DRAW Background
		y_i = 100;
		y_f = y_i + bground.height;
		img_start_pos = (camera_x/5) % bground.width;
		first_chunk_size = bground.width-img_start_pos;
		chunk_size = bground.width;
		last_chunk_size = SCREEN_WIDTH_RES - (first_chunk_size+384);
		last_chunk_size *= (last_chunk_size>0);
		for (int y = y_i; y < y_f; y++) {
			memcpy(draw_buffer + (SCREEN_WIDTH*y) , bground.img_matrix+(y-y_i)*chunk_size+img_start_pos, first_chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size) , bground.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size+chunk_size) , bground.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+first_chunk_size+chunk_size*2) , bground.img_matrix+(y-y_i)*chunk_size, last_chunk_size * sizeof(unsigned int) );
		}

		// Draw bottom 64 dirt
		y_i = SCREEN_HEIGHT-64;
		y_f = y_i + dirt.height;
		img_start_pos = camera_x % dirt.width;
		first_chunk_size = dirt.width-img_start_pos;
		chunk_size = dirt.width;
		last_chunk_size = SCREEN_WIDTH_RES - (192+first_chunk_size+256);
		last_chunk_size *= (last_chunk_size>0);
		for(int y = y_i; y <= y_f; y++) {
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192) , dirt.img_matrix+(y-y_i)*chunk_size+img_start_pos, first_chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192+first_chunk_size) , dirt.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192+first_chunk_size+chunk_size) , dirt.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192+first_chunk_size+chunk_size*2) , dirt.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int) );
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192+first_chunk_size+chunk_size*3) , dirt.img_matrix+(y-y_i)*chunk_size, chunk_size * sizeof(unsigned int));
			memcpy(draw_buffer + (SCREEN_WIDTH*y+192+first_chunk_size+chunk_size*4) , dirt.img_matrix+(y-y_i)*chunk_size, last_chunk_size * sizeof(unsigned int));
		}
		

		for(int y = 100; y <= SCREEN_HEIGHT-64; y++) {
			for(int x = 0; x <= SCREEN_WIDTH_RES; x++) {
				px_index = x + (SCREEN_WIDTH * y);
				val = noise_map[x+camera_x];
				target = &draw_buffer[px_index];
				if (val < y) {
					img_pos_y = (y%64)*64;
					img_pos_x = (camera_x + x) % 64;
					*target = (val < (y-5))*dirt.img_matrix[img_pos_y+img_pos_x] + ((val >= (y-5)))*grass.img_matrix[img_pos_y+img_pos_x];
					continue;
				} 
				if (y>=100+50) {
					*target = 0x41b498;//green grass
				} 
			}
		}
	}
}