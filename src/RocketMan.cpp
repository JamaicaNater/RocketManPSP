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

//Boilerplate PSP code
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
// End of boilerplate PSP code

/**
 * @brief Code written to interrupt the code for the home page
 * 
 * @param args size of argp in bytes
 * @param argp bytes 0-3: ID of the background thread. bytes 4-7: ID of the main thread.
 * @return int 
 */
int interrupt_titlescreen(SceSize args, void* argp){
	unsigned int * arg_arr = (unsigned int*)argp;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	bool in_titlescreen = true;

	while (1)
	{
		sceCtrlReadBufferPositive(&ctrlData, 1);
		if (ctrlData.Buttons & PSP_CTRL_START && in_titlescreen){
			sceKernelTerminateThread(arg_arr[0]);
			sceKernelDeleteThread(arg_arr[0]);
			sceKernelWakeupThread(arg_arr[1]);
			PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Waking thread: %u", arg_arr[1]);
			in_titlescreen = false;
			sceKernelExitDeleteThread(PSP_THREAD_STOPPED);
		}
		sceKernelDelayThread(100);
	}
	return 0;
}

const int MAP_SIZE = 5000;
const int PLAYER_SPEED = 2;
int FRAMETIME = MICROSECONDS / 60;

int main()
{  	
	setupCallbacks();
	pspDebugScreenInit();

	GFX::load_terrain_textures();
	GFX::init();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	SceCtrlData ctrlData;

	unsigned int start_time, end_time;
	
	int cam_pos_x =10,
		cam_pos_y =10;


	unsigned char noise_map[MAP_SIZE];
	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	noise.SetFrequency(.008f);
	//noise.SetDomainWarpAmp(30.0f);
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 170) + 40; // MIN hieght = 40 max hieght = 170 + 40
	}

	Projectile player(Vector2d(10,10));
	player.vector.direction = FORWARD;
	bool cam_aligned = true;

	unsigned int home_thid = sceKernelCreateThread("homescreen_thread", GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);
	if (home_thid >= 0) sceKernelStartThread(home_thid, 0, NULL);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");

	int main_thid = sceKernelGetThreadId();
	if (main_thid<0) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "failed to obtain main thread ID");

	unsigned int args[2] = {home_thid, (unsigned int)main_thid}; // If main_thid is not positive the program terminates
	int inter_home_thid = sceKernelCreateThread("interrupt_homescreen_thread", interrupt_titlescreen, 0x12, 0xaFA0, 0, NULL);
	if (inter_home_thid >= 0) sceKernelStartThread(inter_home_thid, 8, args);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");
	sceKernelSleepThread();

	while (1)
	{

		start_time = sceKernelGetSystemTimeLow(); // For FPS calculation
		pspDebugScreenSetXY(0,0);


		sceCtrlReadBufferPositive(&ctrlData, 1); // For reading in controls 
		
		cam_aligned = (cam_pos_x > 0 && cam_pos_x + SCREEN_WIDTH < MAP_SIZE); // is the player in the center of the screen?
		if (cam_aligned) player.vector.x = 512/2;

		if(ctrlData.Buttons & PSP_CTRL_LEFT){
			cam_pos_x -= cam_aligned * PLAYER_SPEED; // if camaligned update cam position otherwise update player position
			player.vector.x-= !cam_aligned * PLAYER_SPEED;
			player.vector.direction = BACKWARD;
		}

		if(ctrlData.Buttons & PSP_CTRL_RIGHT){
			if (cam_aligned || player.vector.x > 512/2) {
				cam_pos_x+=PLAYER_SPEED;
				cam_aligned = true;
			} else {
				player.vector.x+=PLAYER_SPEED;
			}
			player.vector.direction = FORWARD;
		}

		if(ctrlData.Buttons & PSP_CTRL_UP){ 
			player.vector.set_angle(player.vector.get_angle()+2);
		}
		if(ctrlData.Buttons & PSP_CTRL_DOWN){ 
			player.vector.set_angle(player.vector.get_angle()-2);
		}
		//TODO: Clean up code for readability

		if (cam_aligned) {
			player.vector.y = (int)noise_map[cam_pos_x + 512/2];
		} else {
			player.vector.y = (int)noise_map[player.vector.x];
		}

		GFX::drawTerrain(noise_map, cam_pos_x);
		GFX::drawBMP(player.vector.x+5, player.vector.y-20, player.vector.get_angle(), CENTER_LEFT, player.vector.direction, "assets/player_rocket.bmp", 0, player.weapon);
		GFX::drawBMP(player.vector.x, player.vector.y , 0, CENTER, player.vector.direction, "assets/player.bmp", 0, player.image);
		

		GFX::swapBuffers();
		GFX::clear();

		end_time = sceKernelGetSystemTimeLow();
		printf("fps: %.1f, x: %d, y: %d, angle: %d ", 1 / ((end_time - start_time) / static_cast<float>(1000*1000)), player.vector.x, player.vector.y, player.vector.get_angle());

		sceDisplayWaitVblankStart();
	}
}