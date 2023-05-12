EXE=ww
CC=gcc

ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
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
        endif
    endif
run:
	@echo "Compiling..."
ifeq ($(PLATFORM_OS),OSX)
		$(CC) wireworld.c -o ../$(EXE) -lraylib -g3 -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif
ifeq ($(PLATFORM_OS),LINUX)
		$(CC) wireworld.c -o ../$(EXE) -lraylib -g3 -lGL -lm -lpthread -ldl -lrt
endif
	@echo "Running..."
	../$(EXE)
