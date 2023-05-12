EXE=ww
CC=gcc 
CFLAGS=-Wall -O3 -g3


ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
	    RAYFLAGS=-lraylib -g3 -lGL -lm -lpthread -ldl -lrt
        endif
        ifeq ($(UNAMEOS),FreeBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),OpenBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),NetBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),DragonFly)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
	    RAYFLAGS=-lraylib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
        endif
    endif
all:
	@echo "Compiling..."
	$(CC) wireworld.c -o ../$(EXE) $(RAYFLAGS) $(CFLAGS)
