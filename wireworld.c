#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 10
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

enum cell_state {
  EMPTY,
  HEAD,
  TAIL,
  WIRE
};

enum cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];

void draw2Dgrid(void);

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wire World");
  SetTargetFPS(60);   
  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      draw2Dgrid();
      DrawFPS(0,0);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

void draw2Dgrid(void){
	Color Griddycolor = {50,50,50,255};
	for(int i = 0; i < WINDOW_WIDTH; i+=CELL_SIZE){
		// Vertical Lines
		Vector2 linestart = {i,0};
		Vector2 lineend = {i,WINDOW_HEIGHT};
		DrawLineEx(linestart,lineend,2, Griddycolor);
		for(int j = 0;j < WINDOW_HEIGHT; j+= CELL_SIZE){
			// Horizontal lines
			Vector2 linestart = {0,j};
			Vector2 lineend = {WINDOW_WIDTH,j};
			DrawLineEx(linestart,lineend,2, Griddycolor);
			
		}
	}
}
