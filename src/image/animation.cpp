#include "animation.h"

Image Animation::get_frame(int index) {
    // assert(index < rows*cols, "get_frame(index) "
    // "requested an in bounds value of %d for size %d", index, rows * cols);

    return(Image(height, width, img_matrices + (width * height * index), 
        "Non-Loadable Animation frame"));
}

Animation::Animation(uint32_t _rows, uint32_t _cols, uint32_t _frame_time, 
    const char * _filename
){
    frame_time = _frame_time;
    
    int str_size = strlen(_filename) + 1;
    if (str_size > 64) log(CRITICAL, "Filename %s too long", _filename);
    strncpy(filename, _filename, str_size);

    rows = _rows;
    cols = _cols;
}

Animation::~Animation() {
}