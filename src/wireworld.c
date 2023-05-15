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
 * using DrawPixel(int posX, int posY, Color color); 
 */

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + UI_HEIGHT, "Wire World");
  GuiLoadStyleDark();
  //SetTargetFPS(60);
  HideCursor();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;
  cell_coord drawnCell = {0};
  size_t num_changed_coords = 0;
  cell_coord *changedCoords = malloc(((GRID_WIDTH * GRID_HEIGHT) + 1)*sizeof(cell_coord));
  Color *grid_pixels = malloc((GRID_WIDTH * GRID_HEIGHT) * sizeof(Color));

  for(int i = 0; i < GRID_HEIGHT; i++){
	for(int j = 0; j < GRID_WIDTH; j++){
		size_t pixel_index = i * GRID_WIDTH + j;
	        grid_pixels[pixel_index] = BLACK;	
	}
  }


  // Texture to draw cells on, each cell will 
  // be a single pixel that is then scaled up
  // and shifted to fit onto the existing grid
  RenderTexture2D gametexture = LoadRenderTexture(GRID_WIDTH, GRID_HEIGHT);
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
    else if (IsKeyPressed(KEY_X) && !showSaveWindow){
      clearCells();
      for(int i = 0; i < GRID_HEIGHT; i++){
	for(int j = 0; j < GRID_WIDTH; j++){
		size_t pixel_index = i * GRID_WIDTH + j;
	        grid_pixels[pixel_index] = BLACK;	
	}
      }
      UpdateTexture(gametexture.texture,grid_pixels);
    }
    else if (IsKeyPressed(KEY_S) && !showLoadWindow)
      showSaveWindow = true;
    else if (IsKeyPressed(KEY_L) && !showSaveWindow)
      showLoadWindow = true;
    else if (IsKeyPressed(KEY_P)){
	    Image image = LoadImageFromTexture(gametexture.texture);
	    ExportImage(image, "epic_texture.png");
	    UnloadImage(image); 
    }


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
    
    frame_count++;
    if (frame_count % frames_per_tick == 0) {
      frame_count = 0;
      if (playing) {
        updateGrid(changedCoords, &num_changed_coords);
      }
    }

    // draw cell on left click
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !showSaveWindow &&
        !showLoadWindow && !inUIRegion){
      drawnCell = setCell(selected_cell, draw_state);
      if(drawnCell.x != -1 && drawnCell.y != -1){
	      changedCoords[num_changed_coords] = drawnCell;
	      num_changed_coords += 1;
      }
    }

   

    BeginDrawing();

    ClearBackground(BLACK);

    // change the changed pixels data
    for (int i = 0; i < num_changed_coords; i++){
	cell_coord cur = changedCoords[i];
        grid_pixels[cur.y * GRID_WIDTH + cur.x] = state_colors[cell_grid[cur.x][cur.y]];
    }
    if(num_changed_coords){
	    UpdateTexture(gametexture.texture,grid_pixels);
    }
    //EndTextureMode();
    
    //clear changedCoords
    num_changed_coords = 0;

    BeginMode2D(cam);
    
    //Drawing texture for cells
    DrawTexturePro(gametexture.texture, (Rectangle) {0,0,(float)gametexture.texture.width, (float)gametexture.texture.height},(Rectangle) {0,0,(float)gametexture.texture.width*CELL_SIZE, (float)gametexture.texture.height * CELL_SIZE}, (Vector2){0,0}, 0.0f, WHITE);


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
      
      for(int i = 0; i < GRID_HEIGHT; i++){
	for(int j = 0; j < GRID_WIDTH; j++){
		size_t pixel_index = i * GRID_WIDTH + j;
	        grid_pixels[pixel_index] = state_colors[cell_grid[j][i]];	
	}
      }
      UpdateTexture(gametexture.texture,grid_pixels);
    }

    drawCursor(mousePos);
    EndDrawing();

    // only update grid 
    // after certain amount of 
    // frames, to give control 
    // over sim speed

  }
  CloseWindow();
  return 0;
}
