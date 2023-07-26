ENGINE_PATH := $(shell pwd)/Dependencies/Rengine
all:
	ENGINE_PATH=$(ENGINE_PATH) $(MAKE) -C src

clean:
	$(MAKE) -C src clean

lint:
	find . -name "*.cpp" -print0 | xargs -0 ~/.local/bin/cpplint --linelength=100