SHELL := /bin/bash

all: release

.PHONY: release debug install clean
release:
	mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../ && make -j4 && make install

debug:
	mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../ && make -j4 && make install

install:
	cd build && make install

clean:
	cd build && make clean 2> /dev/null || true && (cat install_manifest.txt; echo) | sh -c 'while read line; do echo "rm -f $$line"; rm -f "$$line"; rmdir --ignore-fail-on-non-empty -p "$${line%/*}" 2> /dev/null || true; done' && cd ../ && rm -rf build
