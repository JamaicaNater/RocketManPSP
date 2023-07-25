all:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

lint:
	find . -name "*.cpp" -print0 | xargs -0 ~/.local/bin/cpplint --linelength=100