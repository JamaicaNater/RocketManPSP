#include <algorithm>

#include "loadbmp.h"
#include "stdlib.h"
#include "math.h"

#include "../utils.h"
#include "../logger/logger.h"

int bmp_mem=0;

unsigned int format_pixel(unsigned int data)
{
    // Used to Format Pixels from ARGB to ABGR for the PSP display
    return (data&0xFF000000) | ((data&0xFF000000)>>24 | (data&0x00FF0000)>>8 | (data&0x0000FF00)<<8 | (data&0x000000FF)<<24)>>8;
}

int load_BMP(Image &img) {
    unsigned int * &buf = img.img_matrix;
    unsigned int &height = img.height;
    unsigned int &width = img.width;

    FILE *fp = fopen(img.filename, "rb");
    PSP_LOGGER::log(PSP_LOGGER::INFO, "loading %s into memory", img.filename);
    if(!fp) {
        PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to open %s: does "
        "the file exist?", img.filename);
    }

    int pixlmap_location;
    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(unsigned int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(&width), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(&height), 1, sizeof(unsigned int), fp);


    fseek(fp, pixlmap_location, SEEK_SET);

    int size = width * height;
    
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Allocating Space");
    if (size * sizeof(unsigned int) > 65536) {
        PSP_LOGGER::log(PSP_LOGGER::WARNING, "Attempting to allocate %f kb "
        "of space", size * sizeof(unsigned int)/1024.0f);
    }

    buf = (unsigned int *)malloc(size * sizeof(unsigned int));
    if (!buf) {
        PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Program failed attpeting to "
        "allocate space for %s: %d x %d is likely too big!", img.filename, width, height);
    }

    PSP_LOGGER::log(PSP_LOGGER::INFO, "Reading file");
    fread((void *)buf, sizeof(unsigned int), size, fp); 

    PSP_LOGGER::log(PSP_LOGGER::INFO, "Formatting pixels");
    for (int i = 0; i < size; i++) {
        buf[i] = format_pixel(buf[i]);
    }

    bmp_mem += size*sizeof(unsigned int);
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Succesfuly loaded %s, %f kb used by "
    "bmps", img.filename, bmp_mem/1024.0f);
    fclose(fp);
    return 1;
}

void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    if(!fp) {
        PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the "
        "file exist?", filename);
    }

    int pixlmap_location;

    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(unsigned int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(width), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(height), 1, sizeof(unsigned int), fp);

    int CHUNK_SIZE = *width * *height;

    int last_chunk = round((*width * *height)/CHUNK_SIZE);
    for (int i = 0; i < last_chunk; i++){
    fseek(fp, pixlmap_location+i*CHUNK_SIZE, SEEK_SET);
    fread((void *)(buf+ i*CHUNK_SIZE), sizeof(unsigned int), CHUNK_SIZE, fp); 
    }

    for (unsigned int i = 0; i < *width * *height; i++) buf[i] = format_pixel(buf[i]);

    int temp[*width];
    unsigned int * front, * back;
    for (int i = 0; i < SCREEN_HEIGHT/2; i++) {
        front = buf + i* SCREEN_WIDTH;
        back = buf + (SCREEN_HEIGHT - i - 1)* SCREEN_WIDTH;

        memcpy((void*)temp, (void*)front, SCREEN_WIDTH*sizeof(unsigned int));
        memcpy((void*)front, (void*)back, SCREEN_WIDTH*sizeof(unsigned int));
        memcpy((void*)back, (void*)temp, SCREEN_WIDTH*sizeof(unsigned int));
    }
    fclose(fp);
}

int load_BMP(Animation &anim) {
    unsigned int &rows = anim.rows;
    unsigned int &cols = anim.cols;

    unsigned int &height = anim.height;
    unsigned int &width = anim.width;
    unsigned int * &buf = anim.img_matrices;
    
    FILE *fp = fopen(anim.filename, "rb");
    PSP_LOGGER::log(PSP_LOGGER::INFO, "loading %s into memory", anim.filename);
    if(!fp) {
        PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to open %s: does the" 
        "file exist?", anim.filename);
    }

    int pixlmap_location;

    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(&width), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(&height), 1, sizeof(unsigned int), fp);

    // Ensure that rows and cols evenly divides height and width
    PSP_LOGGER::assert(!(width % cols), "Width of %d is divisible by" 
    "cols %d", width, cols);
    PSP_LOGGER::assert(!(height % rows), "Height of %d is divisible by" 
    "rows %d", height, rows);

    const int BIG_WIDTH = width;
    const int BIG_HEIGHT = height;
    const int row_size = BIG_HEIGHT/rows;
    const int col_size = BIG_WIDTH/cols;

    // Instead of using height and width of the image we now use height and 
    // of the segment
    width/=cols;
    height/=rows;
    int size = width * height;
  
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Allocating Space");
    unsigned int * buf2 = (unsigned int *)malloc(sizeof(unsigned int) * rows * cols * size);
    if (!buf2) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed memory allocation");
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Allocating Space");
    buf = (unsigned int *)malloc(sizeof(unsigned int) * rows * cols * size);
    if (!buf) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed memory allocation");
   
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Reading file");
    fseek(fp, pixlmap_location, SEEK_SET);
    fread(buf2,sizeof(unsigned int), BIG_HEIGHT * BIG_WIDTH,fp);
    int curr_row, curr_col;
    for (int y = 0; y < BIG_HEIGHT; y++) {
        curr_row = rows -  (y / height) - 1;
        for (int x = 0; x < BIG_WIDTH; x++){
            curr_col = x / col_size;
            buf[size * (curr_row * rows + curr_col) + ( (y % height) * width + x % width)] = buf2[y * BIG_WIDTH + x];
        }
        
    }

    PSP_LOGGER::log(PSP_LOGGER::INFO, "Formatting pixels");
    for (unsigned int i = 0; i < BIG_HEIGHT * BIG_WIDTH; i++){
        buf[i] = format_pixel(buf[i]);
    }
    
    bmp_mem += rows * cols* size*sizeof(unsigned int);
    PSP_LOGGER::log(PSP_LOGGER::INFO, "Successfully loaded %s, %f kb used by "
    "BMPs", anim.filename, bmp_mem/1024.0f);
    
    fclose(fp);
    return 1;
}
