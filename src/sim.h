#ifndef SIM_H
#define SIM_H
#include "raylib.h"
#include "screen.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
// Place contents here
extern Color state_colors[];

extern const char *state_names[];

// __packed__ makes the enum 1 byte instead
// of 4 bytes, improves performance ~40%
typedef enum __attribute__((__packed__)) { WIRE, HEAD, TAIL, EMPTY } cell_state;

typedef struct {
  int x;
  int y;
} cell_coord;

typedef struct {
  cell_coord coord;
  cell_state state;
} cell;

typedef struct {
  uint8_t heads;
  uint8_t tails;
} cell_neighbors;

extern cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
extern bool playing;
extern short frames_per_tick;
extern short frame_count;

void updateGrid(cell_coord *changed_cells, size_t* num_changed_coords);
void clearCells(void);
cell_coord getCellIdx(Vector2 mouse_pos);
cell_coord setCell(cell_coord coordinate, cell_state new_state);
cell_neighbors stateInMoore(int x, int y);
void loadDefault(void);

#endif // !SIM_H
