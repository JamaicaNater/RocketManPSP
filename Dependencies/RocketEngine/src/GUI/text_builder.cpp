#include "GUI/text_builder.h"

#include <cstdlib>

#include "loadbmp.h"
#include "psp_malloc.h"

Animation font = Animation(9, 9, 0, "assets/misc/font.bmp");
bool initialized = false;
int convert_ascii(char c);

void init_text_builder() {
    load_BMP(font);
    initialized = true;
}

// TODO where will this be freed
Image text(const char * txt) {
    assert(initialized, "text builder initalized");

    int index = 0;
    int letter_size = font.height * font.width;
    int length = strlen(txt);

    uint32_t * img_matrix = (uint32_t *)psp_malloc(letter_size
        * length * sizeof(uint32_t));
    if (!img_matrix) {
        log(CRITICAL, "Failed to allocate memory for text builder");
    }

    while (txt[index]) {
        Image letter = font.get_frame(convert_ascii(txt[index]));
        // Draw each letter in the img_matrix
        for (uint32_t y = 0; y < font.height; y++) {
            memcpy(&img_matrix[y*length*font.width + index * font.width],
                &letter.img_matrix[y * font.width],
                font.width * sizeof(uint32_t));
        }

        index++;
    }

    return Image(font.height, font.width*length, img_matrix,
        "Non-Loadable text image");
}

int convert_ascii(char c) {
    int frame_num;
    if (c >= 65 && c <= 90) {
        frame_num = c - 65;
    } else if (c >= 97 && c <= 122) {
        frame_num = c - 97 + 27;
    } else if (c >= 48 && c <= 57) {
        frame_num = c - 48 + 27*2;
    } else {
        switch (c) {
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
            log(CRITICAL, "Failed to match: '%c' in text builder", c);
            break;
        }
    }
    return frame_num;
}

int get_font_width() {
    assert(initialized, "Font was initialized before call to get_font_width");
    return font.width;
}

int get_font_height() {
    assert(initialized, "Font was initialized before call to get_font_height");
    return font.height;
}
