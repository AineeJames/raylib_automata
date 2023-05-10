#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 20
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

Color state_colors[] = {
  YELLOW,
  BLUE,
  RED,
  BLACK
};

const char * state_names[] = {
  "WIRE",
  "HEAD",
  "TAIL",
  "ERASE"
};

typedef enum {
  WIRE,
  HEAD,
  TAIL,
  EMPTY
} cell_state;

typedef struct{
	int x;
	int y;
}cell_coord;

cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];

void clearCells(void);
void draw2Dgrid(void);
void drawCells(void);
cell_coord getCellIdx(Vector2 mouse_pos);
void setCell(cell_coord coordinate, cell_state new_state);
int drawHelpItem(cell_state state, cell_state selected, int x, int y);

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 50, "Wire World");
  SetTargetFPS(60);   
  clearCells();
  cell_state draw_state = EMPTY;
  while(!WindowShouldClose()) {

    if (IsKeyPressed(49)) draw_state = WIRE;
    else if (IsKeyPressed(50)) draw_state = HEAD;
    else if (IsKeyPressed(51)) draw_state = TAIL;
    else if (IsKeyPressed(52)) draw_state = EMPTY;

    cell_coord selected_cell = getCellIdx(GetMousePosition());
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) setCell(selected_cell, draw_state);

    BeginDrawing();
      ClearBackground(BLACK);
      drawCells();
      draw2Dgrid();
      DrawFPS(0,0);
      int offset = 15;
      for (int state = WIRE; state < EMPTY+1; state++) {
        offset += drawHelpItem(state, draw_state, offset, WINDOW_HEIGHT + 15) + 20;
      }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void clearCells(void) {
  for(int i = 0; i < GRID_WIDTH; i++) {
    for(int j = 0; j < GRID_HEIGHT; j++) {
      cell_grid[i][j] = EMPTY;
    }
  }
}

int drawHelpItem(cell_state state, cell_state selected, int x, int y) { 
  char label[50];
  sprintf(label, "%d: %s", state + 1, state_names[state]);
  int width = MeasureText(label, 20);
  DrawRectangle(x - 2, y, MeasureText(label, 20) + 4, 20, state_colors[state]);
  if (state == selected) {
    DrawRectangleLines(x - 5, y - 2, MeasureText(label, 20) + 10, 26, WHITE);
  }
  DrawText(label, x, y, 20, BLACK);
  return width;
}

void setCell(cell_coord coordinate, cell_state new_state){
	if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < GRID_WIDTH && coordinate.y < GRID_HEIGHT){
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

void drawCells(void) {
  for(int i = 0; i < GRID_WIDTH; i++) {
    for(int j = 0; j < GRID_HEIGHT; j++) {
      DrawRectangle(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE, state_colors[cell_grid[i][j]]);
    }
  }
}

void draw2Dgrid(void){
	Color Griddycolor = {50,50,50,255};
	for(int i = 0; i <= WINDOW_WIDTH; i+=CELL_SIZE){
		// Vertical Lines
		Vector2 linestart = {i,0};
		Vector2 lineend = {i,WINDOW_HEIGHT};
		DrawLineEx(linestart,lineend,1, Griddycolor);
		for(int j = 0;j <= WINDOW_HEIGHT; j+= CELL_SIZE){
			// Horizontal lines
			Vector2 linestart = {0,j};
			Vector2 lineend = {WINDOW_WIDTH,j};
			DrawLineEx(linestart,lineend,1, Griddycolor);
			
		}
	}
}
