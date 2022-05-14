TARGET = Tutorial
OBJS = main.o gfx.o OpenSimplexNoise/OpenSimplexNoise.o png/lodepng.o
LIBS = -lstdc++ -lm 
#-lpng -lpspaudiolib -lpspaudio -lpspaudiocodec -lintraFont -lz  -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp -lpspmath

CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti -D_WIN32
ASFLAGS = $(CFLAGS)

#PSP Stuff
BUILD_PRX = 1
PSP_FW_VERSION = 600
PSP_LARGE_MEMORY = 1

PSP_EBOOT_ICON = ICON0.png
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = RocketMan

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak