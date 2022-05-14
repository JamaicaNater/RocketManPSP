#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstring>

#include <cstdlib>

#include "Projectile.hpp"
#include "utils.hpp"

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

const int MAP_SIZE = 1000;
const int PLAYER_SPEED = 2;
int FRAMETIME = MICROSECONDS / 60;

int main()
{
	setupCallbacks();
	pspDebugScreenInit();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	
	GFX::init();
	int cam_pos_x =10,
		cam_pos_y =10;

	int time = 0;

	unsigned char noise_map[MAP_SIZE];

	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	auto start_time = sceKernelGetSystemTimeLow();
	
	unsigned char max = 0;

	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 180) + 40; // MIN hieght = 40 max hieght = 180 + 40
		if (noise_map[i] > max) max = noise_map[i];
	}

	auto end_time = sceKernelGetSystemTimeLow();

	//printf("\nmax: %d\n", (int)max);
	//printf("[Test]: took %u microseconds!\nCan be ran %.03f times per second\n", end_time-start_time,  1.0 * MICROSECONDS / (end_time-start_time));
	Projectile worm(Vector3d(10,10));
	bool cam_misaligned = false;

	while (1)
	{
		auto start_time = sceKernelGetSystemTimeLow();
		//printf("hello");

		sceCtrlReadBufferPositive(&ctrlData, 1);
		
		if (cam_pos_x < 0) {
			cam_misaligned = true;
		} else {
			cam_misaligned = false;
			worm.vector.x = 512/2;
		}

		if(ctrlData.Buttons & PSP_CTRL_LEFT){
			if (!cam_misaligned){
				cam_pos_x-=PLAYER_SPEED;
			} else {
				worm.vector.x-=PLAYER_SPEED;
			}
		}
		if(ctrlData.Buttons & PSP_CTRL_RIGHT){
			if (cam_pos_x < MAP_SIZE){
				cam_pos_x+=PLAYER_SPEED;
			}
		}

		if (!cam_misaligned) {
			worm.vector.y = (int)noise_map[cam_pos_x + 512/2];
		} else {
			worm.vector.y = (int)noise_map[worm.vector.x];
		}

		GFX::drawTerrain(noise_map, cam_pos_x);
		GFX::drawRect(worm.vector.x, worm.vector.y-20, 5, 20, 0xD0BBF8);
		GFX::swapBuffers();

		time++;
		int i =0;
		//while (i < 1000*1000 * 5){ i++; }
		
		auto end_time = sceKernelGetSystemTimeLow();
		if (end_time - start_time > 0){
			
		}
		sceDisplayWaitVblankStart();
	}
	

}