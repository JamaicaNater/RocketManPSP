OBJS = src/gfx.o \
	src/loadbmp.o \
	src/logger.o \
	src/psp_malloc.o \
	src/control_reader.o \
	src/physics/physics.o src/physics/collisions.o src/physics/vector2d.o \
	src/GUI/text_builder.o src/GUI/menu.o src/GUI/component.o \
	src/image/image.o src/image/animation.o src/image/image_utils.o \
	src/object/object_list.o src/object/object.o src/object/object_handler.o


LIBRARY_NAME = librocket_engine.a

PSPSDK=$(shell psp-config --pspsdk-path)

CXX      = psp-g++

# Add PSPSDK includes and libraries.
INCDIR   := $(INCDIR) . $(PSPDEV)/psp/include $(PSPSDK)/include $(ENGINE_PATH)/include
LIBDIR   := $(LIBDIR) . $(PSPDEV)/psp/lib $(PSPSDK)/lib

CFLAGS   := $(addprefix -I,$(INCDIR)) -G0 $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS) $(EXTRA_ARGS)
ASFLAGS  := $(CFLAGS) $(ASFLAGS)

all: $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBRARY_NAME)