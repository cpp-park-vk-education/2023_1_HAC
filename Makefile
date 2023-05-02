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

start_server:
	./server/start.sh

start_gui:
	./gui/start.sh

rebuild_server: clean_server build_server

rebuild_server: clean_gui build_gui