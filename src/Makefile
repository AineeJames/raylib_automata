EXE=ww
CC=gcc 
CFLAGS=-Wall -O3
DEBUGFLAGS=-g3
SOURCES=wireworld.c gui_utils.c
OBJECTS=$(SOURCES:.c=.o)
all: executable

ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
	    	RAYFLAGS=-lraylib -g3 -lGL -lm -lpthread -ldl -lrt
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
	    	RAYFLAGS=-lraylib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
        endif
    endif

debug: CFLAGS += -g3	
debug: EXE=debugww
debug: all

executable: $(OBJECTS)
	@echo "Compiling..."
	$(CC) $(OBJECTS) -o ../$(EXE) $(RAYFLAGS) $(CFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(RAYFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

run: all
	../ww
