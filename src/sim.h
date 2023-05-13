#ifndef SIM_H
#define SIM_H
#include "raylib.h"
#include "screen.h"
#include <stdbool.h>
#include <string.h>
// Place contents here
extern Color state_colors[];

extern const char *state_names[];

typedef enum { WIRE, HEAD, TAIL, EMPTY } cell_state;

typedef struct {
  int x;
  int y;
} cell_coord;

typedef struct {
  cell_coord coord;
  cell_state state;
} cell;

typedef struct {
  int heads;
  int tails;
} cell_neighbors;

extern cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
extern bool playing;
extern int frames_per_tick;
extern int frame_count;

void updateGrid(void);
void clearCells(void);
cell_coord getCellIdx(Vector2 mouse_pos);
void setCell(cell_coord coordinate, cell_state new_state);
cell_neighbors stateInMoore(int x, int y);
void loadDefault(void);

#endif // !SIM_H
