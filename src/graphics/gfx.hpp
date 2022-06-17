#pragma once

#include <cstdint>
#include <pspkerneltypes.h>
#include "../object/object.h"

#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH_RES 480
#define SCREEN_WIDTH 512


namespace GFX 
{
	/**
	 * @brief Sets up draw and display buffers
	 * 
	 */
	void init();

	/**
	 * @brief Masks the Alpha channel and returns false if t has a value greater than 0
	 * 
	 * @param pixel - Pixel we are checking
	 * @return true - Pixel is transparent
	 * @return false Pixel is not transparent
	 */
	bool is_transparent(unsigned int pixel);

	void clear();
	/**
	 * @brief Performs bounds checking to make sure the area we attempt to write to can be written to
	 * 
	 * @param x 
	 * @param y 
	 * @param location x and y converted for use in 1d array
	 * @return true valid position
	 * @return false invalid position
	 */
	bool valid_pixel(int x, int y, int * location);

	void copy_buffers();

	/**
	 * @brief Swaps the draw buffer with the display buffer
	 * 
	 */
	void swapBuffers();

	/**
	 * @brief Draws the backround terrain of the worm
	 * 
	 */
	void drawTerrain();

	void blur_screen();

	void tint_screen(uint32_t tint_factor);

	void draw_progress_bar(int x, int y, int height, int width, int val, int max, uint32_t val_color, uint32_t rem_color);

	void simple_drawBMP(int x, int y, Image &image, bool disp_buf = false);
	
	/**
	 * @brief 
	 * 
	 * @param x -  x position to draw at
	 * @param y -  y position to draw at
	 * @param rot - angle of the object
	 * @param pivot-where we rotate the object about;
	 * @param direction - direction of the object
	 * @param filename 
	 * @param filter - TBD
	 * @param image 
	 */
	void drawBMP(int x, int y, short rot, Position pivot, Flips direction, uint32_t filter, Image &image); // TODO: char direction???

	/**
	 * @brief Creates a looping titlescreen that a separate thread terminates upon start button being pressed
	 * 
	 * @param args - unused requred for threading
	 * @param argp - unused requred for threading
	 * @return int 
	 */
	int do_homescreen(SceSize args, void* argp);

	/**
	 * @brief Loads textures needed for the terrain
	 * 
	 */
	void load_terrain_textures();
}