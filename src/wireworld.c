#include "gui_utils.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
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
 * CLEAN up zoomable code
 * Need to make drawing 
 * cells smarter fps goes down
 * as more cells are drawn
 */

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 50, "Wire World");
  GuiLoadStyleDark();
  //SetTargetFPS(60);
  HideCursor();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;
  // loadDefault();
  clearCells();
  Camera2D cam = {0};
  cam.zoom = 1;
  cam.offset.x = GetScreenWidth() / 2.0f;
  cam.offset.y = GetScreenHeight() / 2.0f;
  Vector2 prevMousePos = GetMousePosition();

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

    float mouseDelta = GetMouseWheelMove();

    float newZoom = cam.zoom + mouseDelta * 0.1f;
    if (newZoom <= 0.1)
        newZoom = 0.1f;

    cam.zoom = newZoom;
    Vector2 mousePos = GetMousePosition();
    Vector2 delta = Vector2Subtract(prevMousePos, mousePos);
    prevMousePos = mousePos;

    for (int i = 0; i < 4; i++) {
      if (CheckCollisionPointRec(mousePos, colorsRects[i])) {
        stateMouseHover = i;
        break;
      } else
        stateMouseHover = -1;
    }

    BeginMode2D(cam);

    cell_coord selected_cell = getCellIdx(mousePos);
    Vector2 mapGrid = GetScreenToWorld2D(GetMousePosition(), cam);
    mapGrid.x = floorf(mapGrid.x / CELL_SIZE) * 1.0f;
    mapGrid.y = floorf(mapGrid.y / CELL_SIZE) * 1.0f;
    selected_cell.x = (int) mapGrid.x;
    selected_cell.y = (int) mapGrid.y;
    EndMode2D();

    if ((stateMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      draw_state = stateMouseHover;
    }

    if (mousePos.y >= WINDOW_HEIGHT) {
      inUIRegion = true;
    } else {
      inUIRegion = false;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        cam.target = GetScreenToWorld2D(Vector2Add(cam.offset, delta),cam);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !showSaveWindow &&
        !showLoadWindow && !inUIRegion)
      setCell(selected_cell, draw_state);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode2D(cam);

    mapGrid = GetScreenToWorld2D(GetMousePosition(), cam);
    mapGrid.x = floorf(mapGrid.x / CELL_SIZE) * 1.0f;
    mapGrid.y = floorf(mapGrid.y / CELL_SIZE) * 1.0f;
    drawCells();
    draw2Dgrid();
    drawBorder();
    if (!inUIRegion) {
      drawSelectedCell(selected_cell, draw_state);
    }

    EndMode2D();

    DrawText(TextFormat("%4.0f %4.0f", mapGrid.x, mapGrid.y),10, 10, 20, WHITE);

    DrawFPS(0, 0);
    int offset = 15;
    for (int state = WIRE; state < EMPTY + 1; state++) {
      offset +=
          drawHelpItem(state, draw_state, offset, WINDOW_HEIGHT + 15) + 20;
    }
    drawSpeed();
    DrawText("x: Clear Screen", offset, WINDOW_HEIGHT + 15, 20, RAYWHITE);
    drawPlayingOrPausedIndicator();
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
  }
  CloseWindow();
  return 0;
}
