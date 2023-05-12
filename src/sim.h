#ifndef SIM_H
#define SIM_H

#define CELL_SIZE 20
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

// Place contents here
Color state_colors[] = {(Color){221, 140, 88, 255}, BLUE, RED, BLACK};

const char *state_names[] = {"WIRE", "HEAD", "TAIL", "ERASE"};

typedef enum { WIRE, HEAD, TAIL, EMPTY } cell_state;

typedef struct {
  int x;
  int y;
} cell_coord;

cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
cell_state next_cell_grid[GRID_WIDTH][GRID_HEIGHT];
#endif // !SIM_H
