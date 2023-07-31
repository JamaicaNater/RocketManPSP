OBJS = graphics/gfx.o \
	bmp/loadbmp.o \
	logger/logger.o \
	memory/psp_malloc.o \
	controls/control_reader.o \
	physics/physics.o physics/collisions.o physics/vector2d.o \
	GUI/text_builder.o GUI/menu.o GUI/component.o \
	image/image.o image/animation.o image/image_utils.o \
	object/object_list.o object/object.o object/object_handler.o

LIBRARY_NAME = librengine.a

PSPSDK=$(shell psp-config --pspsdk-path)

CXX      = psp-g++

# Add PSPSDK includes and libraries.
INCDIR   := $(INCDIR) . $(PSPDEV)/psp/include $(PSPSDK)/include
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