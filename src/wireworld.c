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

#define UI_HEIGHT 50

/* TODO add icon for process
 * CLEAN up zoomable code
 * Need to make drawing 
 * cells smarter fps goes down
 * as more cells are drawn
 * to do this need to initiaize 
 * a texture GRID_WIDTH * GRID_HEIGHT
 * pixels, and draw the pixels when
 * ever we would have drawn on screen
 * that way we just show the texture each
 * frame and only draw on it when necessary
 */

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + UI_HEIGHT, "Wire World");
  GuiLoadStyleDark();
  //SetTargetFPS(60);
  HideCursor();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;

  // Texture to draw cells on, each cell will 
  // be a single pixel that is then scaled up
  // and shifted to fit onto the existing grid
  RenderTexture2D gametexture = LoadRenderTexture(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
  BeginTextureMode(gametexture);
  ClearBackground(BLACK);
  EndTextureMode();

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
    // Capping the zoom so you don't zoom
    // out oo much and get lost
    if (newZoom <= 0.03)
        newZoom = 0.03f;

    cam.zoom = newZoom;

    Vector2 mousePos = GetMousePosition();
    Vector2 delta = Vector2Subtract(prevMousePos, mousePos);
    prevMousePos = mousePos;

    // check if mouse is hovering over buttons
    for (int i = 0; i < 4; i++) {
      if (CheckCollisionPointRec(mousePos, colorsRects[i])) {
        stateMouseHover = i;
        break;
      } else
        stateMouseHover = -1;
    }


    // allows drawing to take place in 
    // world space
    BeginMode2D(cam);

    // get selected cell where mouse is 
    // and translate that to a grid coord
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

    // Check if cursor is on screen or
    // in bottom ui area
    if (mousePos.y >= WINDOW_HEIGHT) {
      inUIRegion = true;
    } else {
      inUIRegion = false;
    }

    // pan camera on right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        cam.target = GetScreenToWorld2D(Vector2Add(cam.offset, delta),cam);
    }
    
    // draw cell on left click
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !showSaveWindow &&
        !showLoadWindow && !inUIRegion)
      setCell(selected_cell, draw_state);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode2D(cam);

    drawCells();

    // only draw grid when zoomed in 
    // When grid is too small it destroys 
    // eyes
    if( newZoom > 0.3f){
    draw2Dgrid();
    }

    drawBorder(newZoom);

    if (!inUIRegion) {
      drawSelectedCell(selected_cell, draw_state);
    }

    EndMode2D();

    // Draw the currently hovered over grid coord
    DrawText(TextFormat("%4.0f %4.0f", mapGrid.x, mapGrid.y),10, 10, 20, WHITE);

    DrawFPS(0, 0);

    drawUIBackground(UI_HEIGHT);

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

    // only update grid 
    // after certain amount of 
    // frames, to give control 
    // over sim speed
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
