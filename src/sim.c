#include "sim.h"

// const Color COPPER_PASTEL = {221, 140, 88, 255};
Color state_colors[] = {(Color){221, 140, 88, 255}, BLUE, RED, BLACK};

const char *state_names[] = {"WIRE", "HEAD", "TAIL", "ERASE"};
cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
cell_state next_cell_grid[GRID_WIDTH][GRID_HEIGHT];

bool playing = false;
int frames_per_tick = 4;
int frame_count = 0;

void clearCells(void) {
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      cell_grid[i][j] = EMPTY;
    }
  }
}

void setCell(cell_coord coordinate, cell_state new_state) {
  if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < GRID_WIDTH &&
      coordinate.y < GRID_HEIGHT) {
    cell_grid[coordinate.x][coordinate.y] = new_state;
  }
}

cell_coord getCellIdx(Vector2 mouse_pos) {
  cell_coord cell_idx = {0, 0};
  cell_idx.x = mouse_pos.x / CELL_SIZE;
  cell_idx.y = mouse_pos.y / CELL_SIZE;
  // TraceLog(LOG_DEBUG, "x,y = (%d,%d)", cell_idx.x, cell_idx.y);
  return cell_idx;
}

int stateInMoore(int x, int y, cell_state target_state) {
  int count = 0;

  // Check all 8 neighboring cells for the target state
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      // Skip the cell itself
      if (i == x && j == y)
        continue;

      // Check if the neighboring cell is in bounds
      if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {
        // Check if the neighboring cell has the target state
        if (cell_grid[i][j] == target_state) {
          count++;
        }
      }
    }
  }

  return count;
}

void updateGrid(void) {
  /*
   * As in all cellular automata, time proceeds in discrete steps called
   * generations (sometimes gens or ticks). Cells behave as follows:
   *
   *     empty → empty,
   *     electron head → electron tail,
   *     electron tail → conductor,
   *     conductor → electron head if exactly one or two of the neighbouring
   * cells are electron heads, otherwise remains conductor.*/
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      if (cell_grid[i][j] == TAIL) {
        next_cell_grid[i][j] = WIRE;
      }
      if (cell_grid[i][j] == HEAD && stateInMoore(i, j, TAIL) > 0) {
        next_cell_grid[i][j] = TAIL;
      }
      int number_of_heads = stateInMoore(i, j, HEAD);
      if (cell_grid[i][j] == WIRE &&
          (number_of_heads == 1 || number_of_heads == 2)) {

        next_cell_grid[i][j] = HEAD;
      }
    }
  }
  memcpy(&cell_grid, &next_cell_grid,
         GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
}
