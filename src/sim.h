#ifndef SIM_H
#define SIM_H
#include "raylib.h"
#include "screen.h"
#include <stdbool.h>
// Place contents here
extern Color state_colors[];

extern const char *state_names[];

typedef enum { WIRE, HEAD, TAIL, EMPTY } cell_state;

typedef struct {
  int x;
  int y;
} cell_coord;

extern cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
extern cell_state next_cell_grid[GRID_WIDTH][GRID_HEIGHT];
extern bool playing;
extern int frames_per_tick;
extern int frame_count;

#endif // !SIM_H
