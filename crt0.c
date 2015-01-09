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

#define BASE 0x08e5d7a0
#define MISSION_ADDRESS 0x08df7e64
#define CONTROLLER_ADDRESS 0x08c8da24

#define CONTROLLER_VALUE_OLD 0x00808021
#define CONTROLLER_VALUE_NEW 0x34104000

PSP_MODULE_INFO("PSP2_PU", 0x1000, 1, 2);

int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));

SceUID thid;
unsigned char gameID[10];

/**
 * Verifies that the game is running.
 */
int findGame(void) {
	SceUID fd;

	fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
	sceIoRead(fd, gameID, 10);
	sceIoClose(fd);

	if (strncmp(gameID, "ULUS-10529", 10) == 0) {
		return 0;
	} else {
		return -1;
	}
}

/**
 * Performs the patch responsible for picking up items.
 */
void performPatch() {
	int i = 0;
	u32 final = 0;

	// Checks to see if you're on a mission.
	if (_lw(MISSION_ADDRESS) == 0) {
		return;
	}

	// Also checks to see if you're on a mission.
	if (_lw(BASE) == 0) {
		return;
	}

	// Acquire the final item address.
	final = _lw(BASE) + 0x824;

	// Simple checks to see if you're standing on an item.
	if (_lw(final) == 1) {
		_sw(CONTROLLER_VALUE_NEW, CONTROLLER_ADDRESS);
	} else {
		_sw(CONTROLLER_VALUE_OLD, CONTROLLER_ADDRESS);
	}
}

/**
 * Main function that does the black magic.
 */
void o_Function() {
	int status = 0;

	// Wait until it's completely booted.
	sceKernelDelayThread(15000000);
	while (!sceKernelFindModuleByName("sceKernelLibrary"))
		sceKernelDelayThread(100000);

	// First we want to confirm that the gameID is correct.
	if (findGame() == -1) {
		sceKernelStopUnloadSelfModule(0, NULL, &status, NULL);
		return;
	}

	// Do the loop-de-loop.
	while (1) {
		performPatch();
		sceKernelDelayThread(1000);
	}

}

int _start(SceSize args, void *argp) {
	thid = sceKernelCreateThread("FunctionThread", &o_Function, 0x18, 0x1000, 0,
			NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
	return 0;
}

int _stop(SceSize args, void *argp) {
	sceKernelTerminateThread(thid);
	return 0;
}
