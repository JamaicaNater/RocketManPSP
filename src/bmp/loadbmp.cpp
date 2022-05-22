#include <algorithm>

#include "loadbmp.h"
#include "stdlib.h"
#include "math.h"
#include "../utils.h"
#include "../logger/logger.h"


int load_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    #ifdef PSP_LOGGING
    if(!fp) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the file exist?", filename);
    #endif

    int pixlmap_location;
    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, 4, fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(width), 1, 4, fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(height), 1, 4, fp);


    fseek(fp, pixlmap_location, SEEK_SET);

    int size = *width * *height;
    if (size > 5000) {
        return 0;
    }
    buf = new unsigned int[size];

    #ifdef PSP_LOGGING
    if(!buf) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "failed to allocate enough memory for %s: %d x %d is likely too big!", filename, *width, *height);
    #endif

    fread((void *)buf, 4, size, fp); 
    
    fclose(fp);
    return 1;
}


	unsigned int format_pixel2(unsigned int data)
	{
		// Used to Format Pixels from ARGB to ABGR for the PSP display
		return (data&0xFF000000) | ((data&0xFF000000)>>24 | (data&0x00FF0000)>>8 | (data&0x0000FF00)<<8 | (data&0x000000FF)<<24)>>8;
	}

void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    #ifdef PSP_LOGGING
    if(!fp) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the file exist?", filename);
    #endif

    int pixlmap_location;

    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, 4, fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(width), 1, 4, fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(height), 1, 4, fp);

    int CHUNK_SIZE = *width * *height;
    int chunk[CHUNK_SIZE];

    int last_chunk = round((*width * *height)/CHUNK_SIZE);
    for (int i = 0; i < last_chunk; i++){
    fseek(fp, pixlmap_location+i*CHUNK_SIZE, SEEK_SET);
    fread((void *)(buf+ i*CHUNK_SIZE), 4, CHUNK_SIZE, fp); 
    }

    for (int i = 0; i < *width * *height; i++) buf[i] = format_pixel2(buf[i]);

    int temp[*width];
    unsigned int * front, * back;
    for (int i = 0; i < 272/2; i++) {
        front = buf + i* 512;
        back = buf + (272 - i - 1)* 512;

        memcpy((void*)temp, (void*)front, 512*4);
        memcpy((void*)front, (void*)back, 512*4);
        memcpy((void*)back, (void*)temp, 512*4);
    }
    fclose(fp);
}
