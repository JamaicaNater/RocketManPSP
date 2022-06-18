TARGET = Tutorial
OBJS = RocketMan.o \
	game/gamestate.o \
	game/utils.o \
	game/interface/pause_menu.o \
	game/object_handler/object_handler.o \
	game/object_handler/enemy_handler.o \
	game/object_handler/projectile_handler.o \
	game/object_handler/animation_handler.o \
	game/object_handler/global_object_manager.o \
	game/object_handler/player_handler.o \
	game/object_handler/explosion_handler.o \
	graphics/gfx.o \
	bmp/loadbmp.o \
	logger/logger.o \
	memory/psp_malloc.o \
	controls/control_reader.o \
	physics/physics.o physics/collisions.o physics/vector2d.o \
	GUI/text_builder.o GUI/menu.o GUI/component.o \
	game/interface/pause_menu.o \
	image/image.o image/animation.o \
	object/object_list.o object/object.o \

LIBS = -lstdc++ -lm 
#-lpng -lpspaudiolib -lpspaudio -lpspaudiocodec -lintraFont -lz  -lpspgum 
#-lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp 
#-lpspmath

CFLAGS = 
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti -O3 -Wall -Wextra #-DPSP_LOGGING
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