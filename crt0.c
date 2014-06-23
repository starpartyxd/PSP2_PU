/*
 *  PSP2_PU
 *
 *  Copyright (C) 2014 Omega2058
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <pspkernel.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspiofilemgr.h>

PSP_MODULE_INFO("PSP2_PU", 0x1000, 1, 1);

int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));

SceUID thid;
unsigned char gameID[10];

unsigned int PATCH[] = {
        0x3C0808E5,
        0x8D0861C0,
        0x3C090080,
        0x35298021,
        0x3C0A3410,
        0x11000004,
        0x354A4000,
        0x8D080824,
        0x55000001,
        0x01404825,
        0x3C0808C9,
        0x03E00008,
        0xAD09DA24,
};

/*
 * Verifies that the game is running.
 */
int findGame(void) {
	int currAttempts = 0;
	SceUID fd;
	
	do {
		fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
		if (currAttempts == 2)
			break;
		currAttempts++;
	} while (fd <= 0);
	
	sceIoRead(fd, gameID, 10);
	sceIoClose(fd);
	
	if (gameID[0] != 0x00)
		if(strcmp(gameID, "ULUS-10529"))
			return 0;
			
	return -1;
}

/*
 * Main function that does the black magic.
 */
void o_Function()
{
	/*
	 * Wait until it's completely booted.
	 */
	sceKernelDelayThread(15000000);
	while(!sceKernelFindModuleByName("sceKernelLibrary"))
		sceKernelDelayThread(100000);
	
	/*
	 * First we want to confirm that the gameID is correct.
	 * We copy it into memory and hook to our patch in user space if it is.
	 */
	if(findGame() == 0) {
		memcpy(0x088006F0, PATCH, sizeof(PATCH));
		*((unsigned int*)0x0880004C) = 0x0A2001BC;
	}
	
	/*
	 * Stop and unload the module since we're finished.
	 */
	int status;
	sceKernelStopUnloadSelfModule(0, NULL, &status, NULL);
	return;
}

int _start(SceSize args, void *argp)
{
	thid = sceKernelCreateThread("FunctionThread", &o_Function, 0x18, 0x1000, 0, NULL);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
	return 0;
}

int _stop(SceSize args, void *argp)
{
 	sceKernelTerminateThread(thid);
	return 0;
}