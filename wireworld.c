#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 10
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib [core] example - basic window");
  SetTargetFPS(60);   
  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(BLACK);
      DrawFPS(0,0);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
