.PHONY: all clean debug install unsintall help
libs ?= libs/*/*
all:
	./rap/scripts/build-rap.sh
	@echo "if everything went ok exec can be found at build/rap"
clean:
	-rm -rf build rapper beatmaker makeself
	-rm ./valgrind-out*

debug:
ifndef filename
	$(error filename missing! use: make debug filename=<file>)
endif
	mkdir -p build
	./rap/scripts/debug.sh "$(filename)" "$(libs)"

install: all
	sudo cp build/rap /usr/local/bin/

uninstall:
	sudo rm /usr/local/bin/rap

help:    # show this help message
	@echo "Available targets:"
	@echo "  all        - standalone rap interpreter + compiler"
	@echo "  clean      - remove build artifacts"
	@echo "  debug      - debug as embedded program"
	@echo "  help       - show this help message"
	@echo "  install    - compile and install"
	@echo "  uninstall  - uninstall"