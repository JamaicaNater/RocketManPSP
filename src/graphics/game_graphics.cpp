#include <cstdint> 

#include <gfx.hpp>
#include <image/image.h>
#include <loadbmp.h>

#include "game_globals.h"

Image dirt("assets/terrain/dirt.bmp");
Image sky("assets/terrain/sky.bmp");
Image grass("assets/terrain/grass.bmp");
Image bground("assets/terrain/background.bmp");

void load_terrain_textures() {
    load_BMP(dirt);
    load_BMP(sky);
    load_BMP(grass);
    load_BMP(bground);		
}

void drawTerrain() {
    unsigned char val;
    uint32_t * target;
    int px_index;
    int img_pos_y;
    int img_pos_x;

    int y_i;
    int y_f;
    int img_start_pos;
    int first_chunk_size;
    int chunk_size;
    int last_chunk_size;

    //TODO: increase readability

    //Draw sky bmp
    y_i = 0;
    y_f = y_i + sky.height;
    img_start_pos = (camera_x/6) % sky.width;
    first_chunk_size = sky.width - img_start_pos;
    chunk_size = sky.width;
    last_chunk_size = SCREEN_WIDTH_RES - (first_chunk_size + 384);
    last_chunk_size *= (last_chunk_size > 0);
    for(int y = y_i; y < y_f; y++){
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y), sky.img_matrix+(y) * sky.width + img_start_pos, first_chunk_size * sizeof(uint32_t));
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size), sky.img_matrix+(y) * sky.width, chunk_size * sizeof(uint32_t));
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size+128), sky.img_matrix + (y) * sky.width, chunk_size * sizeof(uint32_t));
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size+256), sky.img_matrix + (y) * sky.width, chunk_size * sizeof(uint32_t));
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size+384), sky.img_matrix + (y) * sky.width, last_chunk_size * sizeof(uint32_t));
    }

    //Draw Blue sky
    for (int y = 64; y < 100; y++){
        for (int x = 0; x < SCREEN_WIDTH_RES; x++) GFX::draw_buffer[y*SCREEN_WIDTH + x] = 0xba7b44;
    }

    //DRAW Background
    y_i = 100;
    y_f = y_i + bground.height;
    img_start_pos = (camera_x/5) % bground.width;
    first_chunk_size = bground.width-img_start_pos;
    chunk_size = bground.width;
    last_chunk_size = SCREEN_WIDTH_RES - (first_chunk_size+384);
    last_chunk_size *= (last_chunk_size>0);
    for (int y = y_i; y < y_f; y++) {
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y), bground.img_matrix+(y - y_i) * chunk_size + img_start_pos, first_chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size), bground.img_matrix + (y - y_i) * chunk_size, chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size + chunk_size), bground.img_matrix + (y - y_i) * chunk_size, chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + first_chunk_size + chunk_size * 2), bground.img_matrix+(y - y_i) * chunk_size, last_chunk_size * sizeof(uint32_t) );
    }

    // Draw bottom 64 dirt
    y_i = SCREEN_HEIGHT - 64;
    y_f = y_i + dirt.height;
    img_start_pos = camera_x % dirt.width;
    first_chunk_size = dirt.width-img_start_pos;
    chunk_size = dirt.width;
    last_chunk_size = SCREEN_WIDTH_RES - (192 + first_chunk_size + 256);
    last_chunk_size *= (last_chunk_size> 0 );
    for(int y = y_i; y <= y_f; y++) {
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192) , dirt.img_matrix + (y - y_i) * chunk_size + img_start_pos, first_chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192 + first_chunk_size), dirt.img_matrix+(y - y_i) * chunk_size, chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192 + first_chunk_size+chunk_size), dirt.img_matrix + (y - y_i) * chunk_size, chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192 + first_chunk_size+chunk_size * 2), dirt.img_matrix+(y - y_i) * chunk_size, chunk_size * sizeof(uint32_t) );
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192 + first_chunk_size+chunk_size * 3), dirt.img_matrix+(y - y_i) * chunk_size, chunk_size * sizeof(uint32_t));
        memcpy(GFX::draw_buffer + (SCREEN_WIDTH * y + 192 + first_chunk_size+chunk_size * 4), dirt.img_matrix+(y - y_i) * chunk_size, last_chunk_size * sizeof(uint32_t));
    }
    

    for(int y = 100; y <= SCREEN_HEIGHT-64; y++) {
        for(int x = 0; x <= SCREEN_WIDTH_RES; x++) {
            px_index = x + (SCREEN_WIDTH * y);
            val = noise_map[x + camera_x];
            target = &GFX::draw_buffer[px_index];
            if (val < y) {
                img_pos_y = (y % 64) * 64;
                img_pos_x = (camera_x + x) % 64;
                *target = (val < (y - 5)) * dirt.img_matrix[img_pos_y + img_pos_x] + ((val >= (y - 5))) * grass.img_matrix[img_pos_y + img_pos_x];
                continue;
            } 
            if (y>=100 + 50) {
                *target = 0x41b498;//green grass
            } 
        }
    }
}