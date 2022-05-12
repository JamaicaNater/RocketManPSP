#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <pspsysmem_kernel.h>
#include <memory>
#include <cstring>

#include <cstdlib>

#include "gfx.hpp"
#include "OpenSimplexNoise/OpenSimplexNoise.h"
#include "FastNoise/FastNoise.hpp"

#define MICROSECONDS 1000 * 1000
#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("Tutorial", 0, 1, 0);


int exit_calback(int arg1, int arg2, void* common)
{
	sceKernelExitGame();
	return 0;
}

int callbackThread(SceSize args, void* argp)
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_calback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();
	return 0;
}

void setupCallbacks()
{
	int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, NULL);

	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);

}

float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

int main()
{
	setupCallbacks();
	pspDebugScreenInit();

	// sceCtrlSetSamplingCycle(0);
	// sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	
	GFX::init();
	int pos_x = 10,
		pos_y = 10;

	int time = 0;

	int * noisemap = (int*)::operator new(55535);
	int * noisemap2 = (int*)::operator new(25535);
	while (1)
	{
		printf("hello");
		
		// FastNoiseLite noise(sceKernelGetSystemTimeLow());
		// auto start_time = sceKernelGetSystemTimeLow();
		
		// char max = 0;

		// int count = 0;
		// int num = 16 * 16 * 16 * 16 * 16 * 16 * 16;
		// for(float y = 0; y <= 272; y++) {
		// 	for(float x = 0; x < 512; x++) {
		// 		noisemap[static_cast<int>( x + (512 * y) )] = (char)(int)map(noise.GetNoise(x, y), 255);
		// 		char val = noisemap[static_cast<int>( x + (512 * y) )];
		// 		//float val = map(noisemap[static_cast<int>( x + (512 * y) )], 256);
		// 		//printf("%.2f  ", val);
		// 		//if (static_cast<int>(x) % 30 == 0) printf("Ox%08X  ", 0x00FFFFFF + num * static_cast<int>(noisemap[static_cast<int>( x + (512 * y) )]));
		// 		if (val > max) max = val;
		// 	}
		// }

		// auto end_time = sceKernelGetSystemTimeLow();

		// // printf("\nmax: %.03f\n", max);
		// // printf("[Test]: took %u microseconds!\nCan be ran %.03f times per second\n", end_time-start_time,  1.0 * MICROSECONDS / (end_time-start_time));


		// sceCtrlReadBufferPositive(&ctrlData, 1);
		// // while (1)
		// // {
		// // 	/* code */
		// // }
		
		// GFX::drawTerrain(noisemap);
		
		// sceCtrlReadBufferPositive(&ctrlData, 1);
		// // GFX::drawRect(proj.getPostion(time).x, proj.getPostion(time).y, proj.sizex, proj.sizey, 0xFF00FFFF);
		

		// GFX::swapBuffers();

		// time++;
		// int i =0;
		// while (i < 1000*1000 * 30){ i++; }
		
		// sceDisplayWaitVblankStart();
	}
	

}