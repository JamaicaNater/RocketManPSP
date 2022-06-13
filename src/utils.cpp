#include <pspthreadman.h>
#include <cstdlib>
#include "utils/psp_malloc.h"

#include "utils.h"
#include "globals.h"


float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

unsigned int swap_endian(unsigned int data)
{
	return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
}

int get_cam_position(int player_postion, int center) {
	if (player_postion < center) return 0;
	else if (MAP_SIZE - player_postion <= center) return MAP_SIZE - 2*center;

	return player_postion-center;
}

unsigned int filter(unsigned int pixel, unsigned int filter){
    unsigned char red_p = (pixel&0x000000FF), red_f = (filter&0x000000FF),
		green_p = (pixel&0x0000FF00)>>8, green_f = (filter&0x0000FF00)>>8,
		blue_p = (pixel&0x00FF0000)>>16, blue_f = (filter&0x00FF0000)>>16,
		red_r, blue_r, green_r;

	red_r = (red_p + (unsigned int)red_f < UINT8_MAX) ? red_p + red_f : UINT8_MAX;
	green_r = (red_p + (unsigned int)green_f < UINT8_MAX) ? green_p + green_f : UINT8_MAX;
	blue_r = (red_p + (unsigned int)blue_f < UINT8_MAX) ? blue_p + blue_f : UINT8_MAX;

	return red_r | green_r<<8 | blue_r<<16;
}

unsigned int average( unsigned int * arr, int size){
	int sum = 0;
	for (int i = 0; i < size; i++){
		sum += arr[i];
	}
	return sum / size;
}

unsigned int average_pixels( unsigned int arr[], int size){
	unsigned int red_arr[size], blue_arr[size], green_arr[size];
	unsigned char red, green, blue;

	for (int i = 0; i < size; i++){
		red_arr[i] = (arr[i]&0x000000FF);
		green_arr[i] = (arr[i]&0x0000FF00)>>8;
		blue_arr[i] = (arr[i]&0x00FF0000)>>16;
	}

	red = average(red_arr, size);
	blue = average(blue_arr, size);
	green = average(green_arr, size);

	return red | green<<8 | blue<<16;
}

/*
	The following is an implementation of the Box Blur 
	(https://en.wikipedia.org/wiki/Box_blur) algorithm, in the implementation of
	the algorithm we take the neighbors (3 x 3) of a matrix value and average 
	their values together

	This implementation blurs the matrix inplace
*/
void blur(Image &img){
	const unsigned int neighborhood_size_max = 9;
	unsigned int neighborhood_size;
	unsigned int to_avg[neighborhood_size_max];

	int start_i, end_i, start_j, end_j, index;

	unsigned int prev_row[img.width];
	unsigned int prev_item;

	for (unsigned int y = 0; y <= img.height; y++){
		if (y>0) memcpy(prev_row, img.img_matrix + (y-1)*img.width, img.width * sizeof(unsigned int));
		for (unsigned int x = 0; x < img.width; x++){
			if (x>0) prev_item = img.img_matrix[y*img.width + x];		
			start_i = (y > 0) ? -1 : 0;
			start_j = (x > 0) ? -1 : 0;

			end_i = (y < img.height) ? 1 : 0;
			end_j = (x < img.width) ? 1 : 0;

			index = 0;
			for (int i = start_i; i <= end_i; i++){
				for (int j = start_i; j <= end_j; j++){
					// We have already modified the top values
					if (i == -1) to_avg[index] = prev_row[(x+j)]; 
					// We have already modified the left value
					else if (i == 0 && j == -1) to_avg[index] = prev_item;
					// We have not modified these values
					else to_avg[index] = img.img_matrix[(y+i) * img.width + (x+j)];
					index++;
				}
			}
			//PSP_LOGGER::log(PSP_LOGGER::DEBUG, "x %u y %u", x,y);
			neighborhood_size = (end_i - start_i + 1) * (end_j - start_j + 1);
			img.img_matrix[y*img.width + x] = average_pixels(to_avg, neighborhood_size);
		}
	} 
}