#include <algorithm>

#include "loadbmp.h"
#include "stdlib.h"
#include "math.h"
#include "../utils.h"
#include "../logger/logger.h"

unsigned int format_pixel(unsigned int data)
{
    // Used to Format Pixels from ARGB to ABGR for the PSP display
    return (data&0xFF000000) | ((data&0xFF000000)>>24 | (data&0x00FF0000)>>8 | (data&0x0000FF00)<<8 | (data&0x000000FF)<<24)>>8;
}

int load_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "loading %s into memory", filename);
    if(!fp) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the file exist?", filename);

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
    // if (size > 5000) {
    //     PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Image %s of size %d (%d x %d) exceeds the size of any resonable image use write_BMP instead", filename, size, *width, *height);
    // }
    
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Allocating Space");
    try {
        buf = new unsigned int[size];
    } catch (std::exception &e) {
        PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, " Threw exception \"%s\" attpeting to allocate space for %s: %d x %d is likely too big!", e.what(), filename, *width, *height);
    }

    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Reading file");
    fread((void *)buf, 4, size, fp); 

    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Formatting pixels");
    for (int i = 0; i < size; i++) {
        buf[i] = format_pixel(buf[i]);
    }
    
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Succesfuly loaded %s", filename);
    fclose(fp);
    return 1;
}

void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    if(!fp) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the file exist?", filename);

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

    for (int i = 0; i < *width * *height; i++) buf[i] = format_pixel(buf[i]);

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

int load_BMP_array(unsigned int *height,unsigned int *width, 
    unsigned int * * &buf, int rows, int cols, const char * filename) {
    
    FILE *fp = fopen(filename, "rb");
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "loading %s into memory", filename);
    if(!fp) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the file exist?", filename);

    int pixlmap_location;

    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(width), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(height), 1, sizeof(unsigned int), fp);

    // Ensure that rows and cols evenly divides height and width
    PSP_LOGGER::assert_or_log(!(*width % cols), "Width of %d is divisible by" 
    "cols %d", *width, cols);
    PSP_LOGGER::assert_or_log(!(*height % rows), "Height of %d is divisible by" 
    "rows %d", *width, cols);

    // Instead of using height and width of the image we now use height and 
    // of the segment
    *width/=cols;
    *height/=rows;
    int size = *width * *height;

    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Alocating %d pointers for %d x %d images", rows*cols, *width, *height);
    buf = (unsigned int **)malloc(sizeof(unsigned int *) * rows * cols);
    
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Allocating Space");
    for (int i = 0; i < rows * cols; i++) {
        buf[i] = (unsigned int *)malloc(size * sizeof(unsigned int));
        PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "Memory Allocation for pointer %d failed in %s", i, filename);
    }

    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Reading file");
    for(int i = 0; i < *height; i ++){
        fread((unsigned int *)buf[0]+*width*i, sizeof(unsigned int), *width, fp);
        fseek(fp, pixlmap_location + *width * cols*i*sizeof(unsigned int), SEEK_SET);
    } 

    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Formatting pixels");
    for (int i = 0; i < size; i++) {
        buf[0][i] = format_pixel(buf[0][i]);
    }
    
    PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Succesfuly loaded %s", filename);
    fclose(fp);
    return 1;
}
