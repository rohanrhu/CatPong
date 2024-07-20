# CatPong, Multiplayer Pong
# https://github.com/rohanrhu/CatPong
# 
# Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
# Licensed under the MIT License.

BUILD_DATE = "\"$(shell date +'%d.%m.%Y %H:%M')\""
BUILD_NUMBER = $(shell printf $$(($$(cat build.txt) + 1)))

VERBOSE = $(shell printf $$((CATPONG_BUILD_VERBOSE)))

ifeq ($(strip $(VERBOSE)), "")
    VERBOSE := 1
endif

REVISION = "\"$(shell printf $$(git rev-parse --short HEAD))\""

CC = gcc

SDL2_LIB_PATH = /opt/homebrew/Cellar/sdl2/2.30.4/lib
SDL2_TTF_LIB_PATH = /opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib

CFLAGS += -std=c17 \
		 -O0 \
		 -ggdb \
		 -I. \
		 -I./include \
		 -I/opt/homebrew/include \
		 -L"${SDL2_LIB_PATH}" \
		 -lSDL2 \
		 -L"${SDL2_TTF_LIB_PATH}" \
		 -lSDL2_ttf \
		 -Wno-unused-command-line-argument \
		 -Wunused-variable \
		 -Wmissing-field-initializers \
		 -Werror=return-type \
		 \
		 -DCATPONG_VERBOSE=$(VERBOSE) \
		 -DCATPONG_BUILD=$(BUILD_NUMBER) \
		 -DCATPONG_BUILD_DATE=$(BUILD_DATE) \
		 -DCATPONG_REVISION=$(REVISION)

ifneq ($(shell uname -s), Darwin)
    CFLAGS += 
endif

LDFLAGS += 

SOURCES = $(shell find . -wholename "./src/*.c")
HEADERS = $(shell find . -wholename "./include/*.h")
EXECUTABLES = build/catpong
OBJECTS = $(addprefix ./build/, $(notdir $(filter-out ./src/catpong.o, $(SOURCES:.c=.o))))
RM = rm -rf

.PHONY: clean all

all: build/catpong

build/catpong: src/catpong.c $(OBJECTS)
	mkdir -p build/

	$(CC) -o $@ $(filter-out %.h, $^) $(CFLAGS) $(LDFLAGS)
	chmod +x build/catpong

	printf $$(($$(cat build.txt) + 1)) > build.txt
	
	@echo "\033[32mBuild: ${BUILD_NUMBER}\033[0m"
	@echo "\033[32mExecutable: ./build/catpong is built.\033[0m"

build/input.o: src/input.c include/input.h
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/ui.o: src/ui.c include/ui.h
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/util.o: src/util.c include/util.h
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/server.o: src/server.c include/server.h
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/scene_menu.o: src/scene_menu.c
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/scene_singleplayer.o: src/scene_singleplayer.c
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/scene_multiplayer.o: src/scene_multiplayer.c
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

build/pong.o: src/pong.c include/pong.h
	mkdir -p build/
	$(CC) -c -o $@ $(filter-out %.h, $^) $(CFLAGS)

clean:
	$(RM) build/
	$(RM) $(EXECUTABLES)