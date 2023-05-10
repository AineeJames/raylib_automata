EXE=ww
CC=cc

run:
	$(CC) wireworld.c `pkg-config --libs --cflags raylib` -o $(EXE)  
	@echo "Running..."
	./$(EXE)
