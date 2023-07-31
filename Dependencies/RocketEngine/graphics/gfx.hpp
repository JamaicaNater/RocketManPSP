#pragma once

#include <cstdint>
#include <pspkerneltypes.h>

struct Image;

enum Position : uint8_t;
enum Flips : int8_t;

namespace GFX 
{
	extern uint32_t* draw_buffer;
    extern uint32_t* disp_buffer;

	/**
	 * @brief Sets up draw and display buffers
	 * 
	 */
	void init();

	/**
	 * @brief Masks the Alpha channel and returns false if it has a value 
	 * greater than 0
	 * 
	 * @param pixel - Pixel we are checking
	 * @return true - Pixel is transparent
	 * @return false Pixel is not transparent
	 */
	bool is_transparent(uint32_t pixel);

	void clear();
	/**
	 * @brief Performs bounds checking to make sure the area we attempt to write
	 * to can be written to
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
	 * @brief Blurs the screen;
	 * 
	 */
	void blur_screen();

	/**
	 * @brief Tints the screen
	 * 
	 * @param tint_factor 
	 */
	void tint_screen(uint32_t tint_factor);

	/**
	 * @brief Draws a progress bar to the screen
	 * 
	 * @param x 
	 * @param y 
	 * @param height 
	 * @param width 
	 * @param val value of the bar
	 * @param max value value the bar can display
	 * @param val_color color of the value section of the bar
	 * @param rem_color color of the remaining section
	 */
	void draw_progress_bar(int x, int y, int height, int width, int val, 
		int max, uint32_t val_color, uint32_t rem_color);

	/**
	 * @brief Faster draw BMP that uses memcpy
	 * 
	 * @param x 
	 * @param y 
	 * @param image 
	 * @param disp_buf 
	 */
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
	void drawBMP(int x, int y, short rot, Position pivot, Flips direction, 
		uint32_t filter, Image &image); // TODO: char direction???
}