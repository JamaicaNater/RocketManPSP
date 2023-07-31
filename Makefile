ENGINE_PATH := $(shell pwd)/Dependencies/RocketEngine
# EXTRA_ARGS := -DPSP_LOGGING -03
EXTRA_ARGS := -O3
rebuild: clean all

all: 
	EXTRA_ARGS=$(EXTRA_ARGS) $(MAKE) build-engine
	EXTRA_ARGS=$(EXTRA_ARGS) $(MAKE) build-game

clean:
	$(MAKE) clean-engine
	$(MAKE) clean-game

build-game:
	ENGINE_PATH=$(ENGINE_PATH) $(MAKE) -C src

clean-game:
	$(MAKE) -C src clean

build-engine:
	$(MAKE) -C $(ENGINE_PATH)

clean-engine:
	$(MAKE) -C $(ENGINE_PATH) clean

lint:
	find . -name "*.cpp" -print0 | xargs -0 ~/.local/bin/cpplint --linelength=100