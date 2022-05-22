#include <pspthreadman.h>

#include "utils.h"


float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

void wait_for(int micro_secs){
	int start_time = sceKernelGetSystemTimeLow();

	int loop = micro_secs * 32;
	volatile int x;
	for (x = 0; x < loop; x++)
	{
		/*do nothing*/  
		if (sceKernelGetSystemTimeLow() > start_time + micro_secs) break;    
	}
}

unsigned int swap_endian(unsigned int data)
{
	return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
}
