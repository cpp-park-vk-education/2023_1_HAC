.PHONY: all clean_server clean_gui build_server build_gui rebuild_server rebuild_server start_server start_gui

all: clean_server clean_gui build_server build_gui start_server start_gui

clean_server:
	cd server && rm -rf build
clean_gui:
	cd gui && rm -rf build

build_server:
	cd server && ./build.sh

build_gui:
	cd gui && ./build.sh

rebuild_server: clean_server build_server

rebuild_server: clean_gui build_gui

start_server:
	cd server && ./start.sh

start_gui:
	cd gui && ./start.sh

test_server:
	cd server && ./build.sh && ./run_tests.sh

test_gui:
	cd gui && ./build.sh && ./run_tests.sh

memtest_server:
	./server/build.sh -DWITH_MEMCHECK=ON
	./server/run_tests.sh --memcheck

memtest_gui:
	./gui/build.sh -DWITH_MEMCHECK=ON
	./gui/run_tests.sh --memcheck
