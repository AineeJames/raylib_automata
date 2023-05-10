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
void initRandom(void);
void drawCells(void);

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wire World");
  SetTargetFPS(60);   
  initRandom();
  while(!WindowShouldClose()) {
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

void drawCells(void) {
	for(int i = 0; i < WINDOW_WIDTH; i+=CELL_SIZE){
		for(int j = 0;j < WINDOW_HEIGHT; j+= CELL_SIZE){
      DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, state_colors[cell_grid[i][j]]);
    }
  }
}

void initRandom(void){
  for(int i = 0; i < GRID_WIDTH; i++) {
    for(int j = 0; j < GRID_HEIGHT; j++) {
      cell_grid[i][j] = (cell_state)GetRandomValue(0, 3);
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
