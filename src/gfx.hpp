#pragma once

#include <cstdint>
#include <pspkerneltypes.h>
#include "Projectile.hpp"

#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH_RES 480
#define SCREEN_WIDTH 512
#define PI 3.14159265359

enum flips{FORWARD, BACKWARD};

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

	/**
	 * @brief Swaps the draw buffer with the display buffer
	 * 
	 */
	void swapBuffers();

	/**
	 * @brief Draws the backround terrain of the worm
	 * 
	 * @param noise Psudorandom perlin noise 
	 * @param cam_pos_x Position of the camera
	 */
	void drawTerrain(unsigned char noise[], int cam_pos_x);
	
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
	void drawBMP(int x, int y, short rot, char direction, const char* filename, uint32_t filter, Image &image);

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