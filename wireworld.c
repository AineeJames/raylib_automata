#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 10
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

Color state_colors[] = {
  BLACK,
  BLUE,
  RED,
  YELLOW
};

typedef enum {
  EMPTY,
  HEAD,
  TAIL,
  WIRE
} cell_state;

typedef struct{
	int x;
	int y;
}cell_coord;

cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];

void draw2Dgrid(void);
void drawCells(void);
cell_coord getCellIdx(Vector2 mouse_pos);

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wire World");
  SetTargetFPS(60);   
  while(!WindowShouldClose()) {
    cell_coord selected_cell = getCellIdx(GetMousePosition());
    if (IsKeyPressed(49)) TraceLog(LOG_DEBUG, "PRESSED 1");
    if (IsKeyPressed(50)) TraceLog(LOG_DEBUG, "PRESSED 2");
    if (IsKeyPressed(51)) TraceLog(LOG_DEBUG, "PRESSED 3");
    if (IsKeyPressed(52)) TraceLog(LOG_DEBUG, "PRESSED 4");
    BeginDrawing();
      ClearBackground(BLACK);
      drawCells();
      draw2Dgrid();
      DrawFPS(0,0);
    EndDrawing();
  }
  CloseWindow();
  return 0;
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
	for(int i = 0; i < WINDOW_WIDTH; i+=CELL_SIZE){
		// Vertical Lines
		Vector2 linestart = {i,0};
		Vector2 lineend = {i,WINDOW_HEIGHT};
		DrawLineEx(linestart,lineend,1, Griddycolor);
		for(int j = 0;j < WINDOW_HEIGHT; j+= CELL_SIZE){
			// Horizontal lines
			Vector2 linestart = {0,j};
			Vector2 lineend = {WINDOW_WIDTH,j};
			DrawLineEx(linestart,lineend,1, Griddycolor);
			
		}
	}
}
