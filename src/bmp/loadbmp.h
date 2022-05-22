#include <stdio.h>
#include <string.h>

/**
 * @brief 
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