#include "../image/image.h"
/**
 * @brief Loads the Bmp fo the font table into memory
 * 
 */
void init_text_builder();

/**
 * @brief Creates an image out of the text 
 * 
 * @param txt 
 * @return Image 
 */
Image text(const char * txt);