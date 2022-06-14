#include <stdio.h>
#include <string.h>
#include "../object.h"
#include "../animation.h"

//TODO redocument this
/**
 * @brief Opens a bmp file as a binary file and get the width hieght and 
 * pixmap location, after it reads in the pixmap data and stores it
 * 
 * @param img
 * @return int 
 */
int load_BMP(Image &img);

/**
 * @brief to be removed in favor of load BMP, remant from before realized i could increse the memory size
 * 
 * @param height 
 * @param width 
 * @param buf 
 * @param filename 
 */
void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename);

/**
 * @brief Reads in a large image composed of multiple sprites in different 
 * states. These sprite could be part of an animation or just multiple related
 * images we need for a purpose.
 * 
 * @param height height of the large image
 * @param width width of the large image
 * @param buf buffer where the image matrix will be stored
 * @param rows 
 * @param cols 
 * @param filename 
 * @return int 
 */
int load_BMP(Animation &anim);