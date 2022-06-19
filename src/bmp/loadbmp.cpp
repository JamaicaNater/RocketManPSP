#include <algorithm>

#include "loadbmp.h"
#include <stdlib.h>
#include <math.h>

#include "../memory/psp_malloc.h"
#include "../logger/logger.h"

int bmp_mem=0;

int load_BMP(Image &img) {
    FILE *fp = fopen(img.filename, "rb");
    log(INFO, "loading %s into memory", img.filename);
    if(!fp) {
        log(CRITICAL, "Failed to open %s: does the file exist?", img.filename);
    }

    int pixlmap_location;
    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(unsigned int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(&img.width), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(&img.height), 1, sizeof(unsigned int), fp);


    fseek(fp, pixlmap_location, SEEK_SET);

    int size = img.width * img.height;

    img.img_matrix = (unsigned int *)psp_malloc(size * sizeof(unsigned int));
    if (!img.img_matrix) log(CRITICAL, "Failed memory allocation");
    
    log(INFO, "Reading file");
    fread((void *)img.img_matrix, sizeof(unsigned int), size, fp); 

    log(INFO, "Formatting pixels");
    for (int i = 0; i < size; i++) {
        img.img_matrix[i] = format_pixel(img.img_matrix[i]);
    }

    bmp_mem += size*sizeof(unsigned int);
    log(INFO, "Successfully loaded %s, %d kb used by bmps", 
        img.filename, bmp_mem);

    fclose(fp);
    return 1;
}

void write_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");
    if(!fp) {
        log(CRITICAL, "Failed to open %s: does the file exist?", filename);
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
    FILE *fp = fopen(anim.filename, "rb");
    log(INFO, "loading %s into memory", anim.filename);
    if(!fp) {
        log(CRITICAL, "Failed to open %s: does the file exist?", anim.filename);
    }
    int pixlmap_location;
    unsigned int BIG_WIDTH;
    unsigned int BIG_HEIGHT;
    int size;

    // 10 is Location of pixel data in files
    fseek(fp, 10, SEEK_SET);
    fread((void *)(&pixlmap_location), 1, sizeof(int), fp);

    //Load Height and Width info
    fseek(fp, 18, SEEK_SET);
    fread((void *)(&BIG_WIDTH), 1, sizeof(unsigned int), fp);
    fseek(fp, 22, SEEK_SET);
    fread((void *)(&BIG_HEIGHT), 1, sizeof(unsigned int), fp);

    // Ensure that rows and cols evenly divides height and width
    assert(!(anim.width % anim.cols), "Width of %d is divisible by" 
    "cols %d", anim.width, anim.cols);
    assert(!(anim.height % anim.rows), "Height of %d is divisible by" 
    "rows %d", anim.height, anim.rows);

    // Instead of using height and width of the image we now use height and 
    // of the segment
    anim.width = BIG_WIDTH / anim.cols;
    anim.height= BIG_HEIGHT / anim.rows;
    size = anim.width * anim.height;

    log(INFO, "Allocating Space");
    anim.img_matrices = (unsigned int *)psp_malloc(sizeof(unsigned int) 
        * anim.rows * anim.cols * size);
    if (!anim.img_matrices) log(CRITICAL, "Failed memory allocation");
   
    int curr_row;
    for (unsigned int y = 0; y < BIG_HEIGHT; y++) {
        curr_row = anim.rows -  (y / anim.height) - 1;
        for (unsigned int curr_col = 0; curr_col < anim.cols; curr_col++){
            fseek(fp, pixlmap_location + (y * BIG_WIDTH + curr_col * anim.width)
                * sizeof(unsigned int), SEEK_SET);
            fread(&anim.img_matrices[size * (curr_row * anim.rows + curr_col) 
                + ( (y % anim.height) * anim.width)], 
                sizeof(unsigned int), anim.width, fp);
        }
    }

    log(INFO, "Formatting pixels");
    for (unsigned int i = 0; i < BIG_HEIGHT * BIG_WIDTH; i++){
        anim.img_matrices[i] = format_pixel(anim.img_matrices[i]);
    }
    
    bmp_mem += anim.rows * anim.cols* size*sizeof(unsigned int);
    log(INFO, "Successfully loaded %s, %d bytes used by BMPs", 
        anim.filename, bmp_mem);
    
    fclose(fp);
    return 1;
}

uint32_t format_pixel(uint32_t data){
    // Used to Format Pixels from ARGB to ABGR for the PSP display
	// Bit shift was done in this weird way due to the psp being finicky
    return (data&0xFF000000) | ((data&0xFF000000)>>24 | (data&0x00FF0000)>>8 | (data&0x0000FF00)<<8 | (data&0x000000FF)<<24)>>8;
}
