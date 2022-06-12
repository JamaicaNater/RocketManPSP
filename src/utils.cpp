#include <pspthreadman.h>
#include <cstdlib>

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


Image blur(Image img){
	unsigned int * new_mat = (unsigned int *)malloc(img.height * img.width * sizeof(unsigned int));
	for (unsigned int y = 1; y < img.height-1; y++){
		for (unsigned int x = 1; x < img.width-1; x++){
			unsigned int to_avg[9] =
				{img.img_matrix[(y-1) * img.width + (x-1)], img.img_matrix[(y-1) * img.width + x], 
			img.img_matrix[(y-1) * img.width + (x+1)], img.img_matrix[y * img.width + (x-1)],
			img.img_matrix[y * img.width + (x)],
			img.img_matrix[y * img.width + (x+1)], img.img_matrix[(y+1) * img.width + (x-1)], 
			img.img_matrix[(y+1) * img.width + x], img.img_matrix[(y+1) * img.width + (x+1)]};

			new_mat[y*img.width + x] = average_pixels(to_avg, 9);
		}
	}
	return Image(img.height, img.width, new_mat, "blur");
}