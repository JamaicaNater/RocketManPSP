TARGET = Tutorial
OBJS = RocketMan.o \
	gamestate.o \
	graphics/gfx.o \
	bmp/loadbmp.o \
	utils.o \
	logger/logger.o \
	utils/psp_malloc.o \
	controls/control_reader.o \
	physics/physics.o physics/collisions.o \
	object_handler/object_handler.o object_handler/enemy_handler.o \
	object_handler/projectile_handler.o object_handler/animation_handler.o \
	object_handler/global_object_manager.o object_handler/player_handler.o \
	GUI/text_builder.o GUI/menu.o GUI/component.o \
	image/image.o image/animation.o \
	object/object_list.o object/object.o \

LIBS = -lstdc++ -lm 
#-lpng -lpspaudiolib -lpspaudio -lpspaudiocodec -lintraFont -lz  -lpspgum 
#-lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp 
#-lpspmath

CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti -O3 -Wall -Wextra -DPSP_LOGGING
ASFLAGS = $(CFLAGS)

#PSP Stuff
BUILD_PRX = 1
PSP_FW_VERSION = 600
PSP_LARGE_MEMORY = 1

PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_PIC1 = PIC1.png
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = RocketMan

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak