#include <stdio.h>
#include <string.h>

/**
 * @brief Opens a bmp file as a binary file and get the width hieght and 
 * pixmap location, after it reads in the pixmap data and stores it
 * 
 * @param height 
 * @param width 
 * @param buf 
 * @param filename 
 * @return int 
 */
int load_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename);

/**
 * @brief 
 * 
 * @param height 
 * @param width 
 * @param buf 
 * @param filename 
 */
void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename);