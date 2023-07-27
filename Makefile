ENGINE_PATH := $(shell pwd)/Dependencies/Rengine

all: 
	$(MAKE) build-engine
	$(MAKE) build-game

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