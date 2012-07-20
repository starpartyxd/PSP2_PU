TARGET = PSP2_PU
OBJS = crt0.o exports.o

BUILD_PRX=1
PSP_FW_VERSION = 150
PRX_EXPORTS=exports.exp

USE_KERNEL_LIBS = 1
USE_KERNEL_LIBC = 1

INCDIR =
CFLAGS = -O2 -G0 -w -msingle-float -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LIBS = -lpspchnnlsv -lpsputility -lpspdebug -lpspge_driver -lpspwlan -lpspumd -lpsppower -lpspwlan -lpspnet -lpsprtc -lpsphprm

LDFLAGS = -nostdlib  -nodefaultlibs -g

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

lite:
	psp-packer $(TARGET).prx