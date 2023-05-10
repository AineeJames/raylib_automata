EXE=ww
CC=gcc

run: clean
	@echo "Compiling..."
	$(CC) wireworld.c -o $(EXE) -lraylib  
	@echo "Running..."
	./$(EXE)

clean:
	rm -f $(EXE)
