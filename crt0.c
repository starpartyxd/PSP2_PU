#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>

PSP_MODULE_INFO("PSP2_PU", 0x3008, 1, 2);
PSP_MAIN_THREAD_ATTR(0); 

int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));

SceUID thid;
unsigned int *p0 = (unsigned int) (0x088006f0);
unsigned int *p1 = (unsigned int) (0x088006f4);
unsigned int *p2 = (unsigned int) (0x088006f8);
unsigned int *p3 = (unsigned int) (0x088006fc);
unsigned int *p4 = (unsigned int) (0x08800700);
unsigned int *p6 = (unsigned int) (0x08800708);
unsigned int *p7 = (unsigned int) (0x0880070c);
unsigned int *p9 = (unsigned int) (0x08800714);
unsigned int *p10 = (unsigned int) (0x08800718);
unsigned int *p11 = (unsigned int) (0x0880071c);
unsigned int *p12 = (unsigned int) (0x08800720);
unsigned int *p13 = (unsigned int) (0x08800724);
unsigned int *p14 = (unsigned int) (0x08800728);
unsigned int *p15 = (unsigned int) (0x0880072c);
unsigned int *p16 = (unsigned int) (0x08800730);
unsigned int *p17 = (unsigned int) (0x08800734);
unsigned int *p18 = (unsigned int) (0x0880004c);

void Function()
{
  sceKernelDelayThread(15000000);
  while(!sceKernelFindModuleByName("sceKernelLibrary"))
  sceKernelDelayThread(100000);

  // Quick and dirty, just write instructions and patch the hook.
  // Whatever works, right?
  
  *p0 = (unsigned int) (0x3c0808e5);
  *p1 = (unsigned int) (0x8d0861c0);
  *p2 = (unsigned int) (0x3c0a08c8);
  *p3 = (unsigned int) (0x354ada24);
  *p4 = (unsigned int) (0x11000009);
  *p6 = (unsigned int) (0x21080824);
  *p7 = (unsigned int) (0x8d080000);
  *p9 = (unsigned int) (0x11000004);
  *p10 = (unsigned int) (0x3c0b3410);
  *p11 = (unsigned int) (0x356b4000);
  *p12 = (unsigned int) (0xad4b0000);
  *p13 = (unsigned int) (0x03e00008);
  *p14 = (unsigned int) (0x3c0c0080);
  *p15 = (unsigned int) (0x358c8021);
  *p16 = (unsigned int) (0xad4c0000);
  *p17 = (unsigned int) (0x03e00008);
  *p18 = (unsigned int) (0x0a2001bc);
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
