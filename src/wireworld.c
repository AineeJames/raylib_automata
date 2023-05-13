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

/* TODO add icon for process
 *
 *
 */

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 50, "Wire World");
  GuiLoadStyleDark();
  SetTargetFPS(60);
  HideCursor();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;
  loadDefault();
  memcpy(&next_cell_grid, &cell_grid,
         GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));

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

    frame_count++;
    if (frame_count % frames_per_tick == 0) {
      frame_count = 0;
      if (playing) {
        updateGrid();
      }
    }
    memcpy(&next_cell_grid, &cell_grid,
           GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
  }
  CloseWindow();
  return 0;
}
