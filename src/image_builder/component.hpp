#pragma once

#include <cstdlib>
#include "../image.hpp"
#include "../logger/logger.h"
#include "../graphics/gfx.hpp"
#include "../utils.h"
#include "utils/psp_malloc.h"


struct Component
{
public:
    enum ComponentType{PANEL, BUTTON, LABEL, EXTERN_IMG};
    
    unsigned int x, y, height, width;
    char * text_field;
    //Component * children[4];

    unsigned int * img_matrix;

    Component(unsigned int _x, unsigned int _y, unsigned int _height, unsigned int _width, unsigned int color){
        x = _x;
        y = _y;
        height = _height;
        width = _width;

        img_matrix = (unsigned int *)psp_malloc(width * height * sizeof(unsigned int));
        for (unsigned int i = 0; i < width * height; i++) img_matrix[i] = color;
    }
    ~Component(){
    //TODO DFS style dealloc
    }

    void add_panel(unsigned int _x, unsigned int _y, unsigned int _height, unsigned int _width, unsigned int _color){
        for (unsigned int y = _x; y < _height + _y; y++){
            for (unsigned int x = _y; x < _width + _x; x++) img_matrix[width*y + x] = tint_pixel(_color, 0x8800D5);
        }
    }

    void add_img(unsigned int _x, unsigned int _y, Image _img){
        for (unsigned int y = _x; y < _img.height + _y; y++){
            for (unsigned int x = _y; x < _img.width + _x; x++) {
                if (!GFX::is_transparent(_img.img_matrix[_img.width*(y - _y) + (x - _x)]) && y < height && x < width)
                img_matrix[width*y + x] = _img.img_matrix[_img.width*(y - _y) + (x - _x)];
            }
        }
    }

    void set_pos(pivots pos, int padding_x = 0, int padding_y = 0){
        int scr_cen_x = SCREEN_WIDTH_RES/2;
        int scr_cen_y = SCREEN_HEIGHT/2;

        switch (pos)
			{
			case CENTER:
				x = scr_cen_x - width/2 + padding_x;
                y = scr_cen_y - height/2 + padding_y;
				break;
			case CENTER_LEFT:
				x = 0 + padding_x;
                y = scr_cen_y - height/2 + padding_y;
				break;
			case CENTER_RIGHT:
				x = SCREEN_WIDTH_RES - width + padding_x;
                y = scr_cen_y - height/2 + padding_y;
				break;
			case TOP_CENTER:
				x = scr_cen_x - width/2 + padding_x;
                y = 0 + padding_y;
				break;
			case BOTTOM_CENTER:
				x = scr_cen_x - width/2 + padding_x;
                y = SCREEN_HEIGHT - height + padding_y;
				break;
			case TOP_LEFT:
				x = 0 + padding_x;
                y = 0 + padding_y;
				break;
			case TOP_RIGHT:
				x = SCREEN_WIDTH_RES - width + padding_x;
                y = 0 + padding_y;
				break;
			\
			default:
				PSP_LOGGER::log(PSP_LOGGER::CRITICAL, "Failed to match a pivot"
					"");
				break;
			}
    }

    Image get_image() {
        return Image(height, width, img_matrix, "component");
    }
};
