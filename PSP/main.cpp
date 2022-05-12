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


struct Vector3d
{
	int x, y;
	float angle;

	Vector3d(int _x, int _y, int _z, float _angle) {
		x = _x;
		y = _y;
		angle = _angle; 
	}

	Vector3d(int _x, int _y) {
		x = _x;
		y = _y;
	}

	Vector3d(){}
};

struct Projectile
{
private:
	int grav = 9.81;

public:
	Vector3d vector;
 	Projectile(Vector3d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};

float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

const int MAP_SIZE = 1000;

int main()
{
	setupCallbacks();
	pspDebugScreenInit();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	
	GFX::init();
	unsigned int cam_pos_x = 10,
		cam_pos_y = 10;

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

	while (1)
	{
		
		//printf("hello");

		sceCtrlReadBufferPositive(&ctrlData, 1);
		if(ctrlData.Buttons & PSP_CTRL_LEFT){
			if (cam_pos_x > 0){
				cam_pos_x-=1;
				worm.vector.x = cam_pos_x;
			}
		}
		if(ctrlData.Buttons & PSP_CTRL_RIGHT){
			if (cam_pos_x < MAP_SIZE){
				cam_pos_x+=1;
				worm.vector.x= cam_pos_x;
			}
		}
		worm.vector.y = noise_map[cam_pos_x];

		GFX::drawTerrain(noise_map, cam_pos_x);
		GFX::drawRect(cam_pos_x, cam_pos_y, 5, 20, 0xD0BBF8);
		GFX::swapBuffers();

		time++;
		int i =0;
		//while (i < 1000*1000 * 5){ i++; }
		
		sceDisplayWaitVblankStart();
	}
	

}