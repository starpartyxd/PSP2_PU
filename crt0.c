#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>

PSP_MODULE_INFO("PSP2_PU", 0x1000, 1, 2);
PSP_MAIN_THREAD_ATTR(0); 

int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));
SceUID thid;

void Function()
{
  sceKernelDelayThread(15000000);
  while(!sceKernelFindModuleByName("sceKernelLibrary"))
  sceKernelDelayThread(100000);
  
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
   
    memcpy(0x088006F0, PATCH, sizeof(PATCH));
    *((unsigned int*)0x0880004C) = 0x0A2001BC;
  return;
}

int _start(SceSize args, void *argp)
{
	thid=sceKernelCreateThread("FunctionThread", &Function, 0x18, 0x1000, 0, NULL);
	if(thid >= 0) sceKernelStartThread(thid, 0, NULL);
	return 0;
}

int _stop(SceSize args, void *argp)
{
 	sceKernelTerminateThread(thid);
	return 0;
}
