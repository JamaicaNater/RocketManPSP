#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstring>

#include <cstdlib>

#include "Projectile.hpp"
#include "utils.h"
#include "logger/logger.h"

#include "gfx.hpp"
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

int interrupt_homescreen(SceSize args, void* argp){
	unsigned int * arg_arr = (unsigned int*)argp;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	bool in_homescreen = true;

	while (1)
	{
		sceCtrlReadBufferPositive(&ctrlData, 1);
		if (ctrlData.Buttons & PSP_CTRL_START && in_homescreen){
			sceKernelTerminateThread(arg_arr[0]);
			sceKernelDeleteThread(arg_arr[0]);
			sceKernelWakeupThread(arg_arr[1]);
			PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Waking thread: %u", arg_arr[1]);
			sceKernelExitDeleteThread(PSP_THREAD_STOPPED);
			in_homescreen = false;
		}
		sceKernelDelayThread(100);
	}
	return 0;
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

	GFX::load_terrain_textures();

	unsigned int start_time, end_time;

	SceCtrlData ctrlData;
	
	GFX::init();
	int cam_pos_x =10,
		cam_pos_y =10;


	unsigned char noise_map[MAP_SIZE];
	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 170) + 40; // MIN hieght = 40 max hieght = 170 + 40
	}


	Projectile worm(Vector2d(10,10));
	worm.vector.direction = FORWARD;
	//worm.vector.angle = 45;
	
	bool cam_aligned = true;

	unsigned int thid = sceKernelCreateThread("homescreen_thread", GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);
	if (thid >= 0) sceKernelStartThread(thid, 0, NULL);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");



	unsigned int args[2] = {thid, sceKernelGetThreadId()};
	int thid2 = sceKernelCreateThread("interrupt_homescreen_thread", interrupt_homescreen, 0x12, 0xaFA0, 0, NULL);
	if (thid2 >= 0) sceKernelStartThread(thid2, 8, args);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");

	sceKernelSleepThread();
	while (1)
	{
		pspDebugScreenSetXY(0,0);
		start_time = sceKernelGetSystemTimeLow();
		sceCtrlReadBufferPositive(&ctrlData, 1);
		
		cam_aligned = (cam_pos_x > 0 && cam_pos_x + SCREEN_WIDTH < MAP_SIZE);
		if (cam_aligned) worm.vector.x = 512/2;

		if(ctrlData.Buttons & PSP_CTRL_LEFT){
			cam_pos_x -= cam_aligned * PLAYER_SPEED; // if camaligned update cam position otherwise update worm position
			worm.vector.x-= !cam_aligned * PLAYER_SPEED;
			worm.vector.direction = BACKWARD;
		}

		if(ctrlData.Buttons & PSP_CTRL_RIGHT){
			if (cam_aligned || worm.vector.x > 512/2) {
				cam_pos_x+=PLAYER_SPEED;
				cam_aligned = true;
			} else {
				worm.vector.x+=PLAYER_SPEED;
			}
			worm.vector.direction = FORWARD;
		}

		if(ctrlData.Buttons & PSP_CTRL_UP){ 
			worm.vector.set_angle(worm.vector.get_angle()+2);
		}
		if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
			worm.vector.set_angle(worm.vector.get_angle()-2);
		}
		//TODO: Clean up code for readability

		if (cam_aligned) {
			worm.vector.y = (int)noise_map[cam_pos_x + 512/2];
		} else {
			worm.vector.y = (int)noise_map[worm.vector.x];
		}

		GFX::drawTerrain(noise_map, cam_pos_x);
		GFX::drawBMP(worm.vector.x+5, worm.vector.y-20, worm.vector.get_angle(), worm.vector.direction, "assets/player_rocket.bmp", 0, worm.weapon);
		GFX::drawBMP(worm.vector.x, worm.vector.y , 0, worm.vector.direction, "assets/player.bmp", 0, worm.image);
		

		GFX::swapBuffers();
		end_time = sceKernelGetSystemTimeLow();
		printf("fps: %.1f, x: %d, y: %d, angle: %d ", 1 / ((end_time - start_time) / static_cast<float>(1000*1000)), worm.vector.x, worm.vector.y, worm.vector.get_angle());
		sceDisplayWaitVblankStart();
	}
}