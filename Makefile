.PHONY: all clean_server clean_gui build_server build_gui rebuild_server rebuild_server start_server start_gui

all: clean_server clean_gui build_server build_gui start_server start_gui

clean_server:
	rm -rf ./server/build
clean_gui:
	rm -rf ./gui/build

build_server:
	./server/build.sh

build_gui:
	./gui/build.sh


rebuild_server: clean_server build_server

rebuild_server: clean_gui build_gui


start_server:
	./server/start.sh

start_gui:
	./gui/start.sh

test_server:
	./server/build.sh
	./server/run_tests.sh

test_gui:
	./gui/build.sh
	./gui/run_tests.sh

memtest_server:
	./server/build.sh -DWITH_MEMCHECK=ON
	./server/run_tests.sh --memcheck

memtest_gui:
	./gui/build.sh -DWITH_MEMCHECK=ON
	./gui/run_tests.sh --memcheck

