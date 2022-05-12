TARGET = Tutorial
OBJS = main.o gfx.o OpenSimplexNoise/OpenSimplexNoise.o 
LIBS = -lstdc++ -lm 
#-lpng -lpspaudiolib -lpspaudio -lpspaudiocodec -lintraFont -lz  -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp -lpspmath

CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti 
ASFLAGS = $(CFLAGS)

#PSP Stuff
BUILD_PRX = 1
PSP_FW_VERSION = 600
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Our Craft

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak