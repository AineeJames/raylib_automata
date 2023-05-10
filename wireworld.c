#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib [core] example - basic window");
  SetTargetFPS(60);   
  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
