TARGET = Tutorial
OBJS = RocketMan.o \
	game/gamestate.o \
	game/game_utils.o \
	game/interface/pause_menu.o \
	game/object_handler/enemy_handler.o \
	game/object_handler/projectile_handler.o \
	game/object_handler/animation_handler.o \
	game/object_handler/global_object_manager.o \
	game/object_handler/player_handler.o \
	game/object_handler/explosion_handler.o \
	game/interface/pause_menu.o \
	game/interface/title_menu.o \
	game/interface/main_menu.o \
	game/interface/on_screen.o \
	game/interface/death_menu.o \
	game/graphics/game_graphics.o \

LIBS = -lrocket_engine -lstdc++ -lm 

#-lpng -lpspaudiolib -lpspaudio -lpspaudiocodec -lintraFont -lz  -lpspgum
#-lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp
#-lpspmath

INCDIR   := $(INCDIR) . $(ENGINE_PATH)
LDFLAGS := $(addprefix -L,$(LIBDIR) $(ENGINE_PATH))

CFLAGS =
CXXFLAGS = $(CFLAGS) -std=c++14 -fno-rtti -Wall -Wextra -Wdouble-promotion \
					-Wno-unused-parameter -Warray-bounds -Wcoverage-mismatch \
					-Wparentheses -Wunused-const-variable -Wuseless-cast \
					-Wunused $(EXTRA_ARGS)
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