#include "bmp/loadbmp.h"


int load_BMP(unsigned int *height,unsigned int *width, unsigned int * &buf, const char * filename) {
    FILE *fp = fopen(filename, "rb");

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

    int size = 2000;
    if (size > 5000) {
        return 0;
    }
    buf = new unsigned int[size];

    fread((void *)buf, 4, size, fp); 
    
    fclose(fp);
    return 1;
}