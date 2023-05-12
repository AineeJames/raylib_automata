#include "sim.h"
Color state_colors[] = {(Color){221, 140, 88, 255}, BLUE, RED, BLACK};

const char *state_names[] = {"WIRE", "HEAD", "TAIL", "ERASE"};
cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
cell_state next_cell_grid[GRID_WIDTH][GRID_HEIGHT];

bool playing = false;
int frames_per_tick = 4;
int frame_count = 0;
