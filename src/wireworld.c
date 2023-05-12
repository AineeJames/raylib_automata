#include "gui_utils.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "sim.h"
#include "styles/style_dark.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * If much larger cell grid is desired
 * may need to improve rectangle grid rendering
 * for example with cell_size 5 we have 160x78
 * which is 12480 calls to drawRectangle
 * Instead maybe writing to a texture may
 * be more efficient?
 * */

// const Color COPPER_PASTEL = {221, 140, 88, 255};
bool playing = false;
int frames_per_tick = 4;
int frame_count = 0;

void clearCells(void);
void updateGrid(void);
cell_coord getCellIdx(Vector2 mouse_pos);
void setCell(cell_coord coordinate, cell_state new_state);
int stateInMoore(int x, int y, cell_state target_state);

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 50, "Wire World");
  GuiLoadStyleDark();
  SetTargetFPS(60);
  HideCursor();
  clearCells();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ONE))
      draw_state = WIRE;
    else if (IsKeyPressed(KEY_TWO))
      draw_state = HEAD;
    else if (IsKeyPressed(KEY_THREE))
      draw_state = TAIL;
    else if (IsKeyPressed(KEY_FOUR))
      draw_state = EMPTY;
    else if (IsKeyPressed(KEY_SPACE))
      playing = !playing;
    else if (IsKeyPressed(KEY_UP) && frames_per_tick > 1)
      frames_per_tick--;
    else if (IsKeyPressed(KEY_DOWN))
      frames_per_tick++;
    else if (IsKeyPressed(KEY_X) && !showSaveWindow)
      clearCells();
    else if (IsKeyPressed(KEY_S) && !showLoadWindow)
      showSaveWindow = true;
    else if (IsKeyPressed(KEY_L) && !showSaveWindow)
      showLoadWindow = true;
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 4; i++) {
      if (CheckCollisionPointRec(mousePos, colorsRects[i])) {
        stateMouseHover = i;
        break;
      } else
        stateMouseHover = -1;
    }

    cell_coord selected_cell = getCellIdx(mousePos);
    if ((stateMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      draw_state = stateMouseHover;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !showSaveWindow &&
        !showLoadWindow)
      setCell(selected_cell, draw_state);

    BeginDrawing();
    ClearBackground(BLACK);
    drawCells();
    draw2Dgrid();
    DrawFPS(0, 0);
    int offset = 15;
    for (int state = WIRE; state < EMPTY + 1; state++) {
      offset +=
          drawHelpItem(state, draw_state, offset, WINDOW_HEIGHT + 15) + 20;
    }
    drawSpeed();
    DrawText("x: Clear Screen", offset, WINDOW_HEIGHT + 15, 20, RAYWHITE);
    drawPlayingOrPausedIndicator();
    drawSelectedCell(selected_cell, draw_state);
    if (showSaveWindow) {
      savePopUp();
    }

    if (showLoadWindow) {
      loadPopUp();
    }

    drawCursor(mousePos);
    EndDrawing();

    memcpy(&next_cell_grid, &cell_grid,
           GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
    frame_count++;
    if (frame_count % frames_per_tick == 0) {
      frame_count = 0;
      if (playing) {
        updateGrid();
      }
    }
  }
  CloseWindow();
  return 0;
}

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
