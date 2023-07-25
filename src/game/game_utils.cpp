#include <pspthreadman.h>
#include <cstdlib>
#include "memory/psp_malloc.h"

#include "game_utils.h"
#include "globals.h"

float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

uint32_t swap_endian(uint32_t data)
{
	return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
}

int get_cam_position(int player_postion, int center) {
	if (player_postion < center) return 0;
	else if (MAP_SIZE - player_postion <= center) return MAP_SIZE - 2*center;

	return player_postion-center;
}

uint32_t get_time(){
	return sceKernelGetSystemTimeLow() - pause_time;
}