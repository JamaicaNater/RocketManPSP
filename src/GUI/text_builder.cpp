#include "../bmp/loadbmp.h"
#include "../utils/psp_malloc.h"
#include "text_builder.h"
#include <cstdlib>

Animation font = Animation(9, 9, 0, "assets/font.bmp");
bool initialized = false;
int convert_ascii(char c);

void init_text_builder(){
    load_BMP(font);
    initialized = true;
}

// TODO where will this be freed
Image text(const char * txt) {
    PSP_LOGGER::assert(initialized, "text builder initalized");

    int index = 0;
    int letter_size = font.height * font.width;
    int length = strlen(txt);

    unsigned int * img_matrix = (unsigned int *)psp_malloc( letter_size
        * length * sizeof(unsigned int));
    if(!img_matrix) PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to allocate"
        "memory for text builder");

    while (txt[index])
    {
        // Draw each letter in the img_matrix
        for (unsigned int y = 0; y < font.height; y++){
            memcpy(&img_matrix[y*length*font.width + index * font.width], 
                &font.get_frame(convert_ascii(txt[index])).img_matrix[y * font.width],
                font.width * sizeof(unsigned int));
        }
        
        index++;
    }
    
    return Image(font.height, font.width*length, img_matrix, "Non-Loadable text image");
}

int convert_ascii(char c){
    int frame_num;
    if (c >= 65 && c <= 90) frame_num = c - 65;
        else if (c >= 97 && c <= 122) frame_num = c - 97 + 27;
        else if (c >= 48 && c <= 57) frame_num = c - 48 + 27*2;
        else {
            switch (c)
            {
            case ' ':
                frame_num = 26;
                break;

            case '.':
                frame_num = 64;
                break;

            case ',':
                frame_num = 65;
                break;
            
            case ';':
                frame_num = 66;
                break;
                
            case ':':
                frame_num = 67;
                break;

            case '$':
                frame_num = 68;
                break;

            case '#':
                frame_num = 69;
                break;

            case '\'':
                frame_num = 70;
                break;

            case '!':
                frame_num = 71;
                break;
            
            case '"':
                frame_num = 72;
                break;
                
            case '/':
                frame_num = 73;
                break;

            case '?':
                frame_num = 74;
                break;

            case '%':
                frame_num = 75;
                break;

            case '(':
                frame_num = 76;
                break;

            case ')':
                frame_num = 77;
                break;
            
            case '@':
                frame_num = 78;
                break;           
            default:
                frame_num = 0;
                PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to match: '%c' "
                    "in text builder", c);
                break;
            }
        }
        return frame_num;
} 

int get_font_width(){
    PSP_LOGGER::assert(initialized, "Font was initialized before call to get_font_width");
    return font.width;
}

int get_font_height(){
    PSP_LOGGER::assert(initialized, "Font was initialized before call to get_font_height");
    return font.height;
}