#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib [core] example - basic window");
  SetTargetFPS(60);   
  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawFPS(0,0);
      int broslength = MeasureText("BRUH MOMENT", 20);
      char brodebug[100];
      sprintf(brodebug, "Bro is %d long", broslength);
      DrawText(&brodebug, 100,100, 20, BLACK);
      DrawText("BRUH MOMENT", (WINDOW_WIDTH / 2) - broslength,(WINDOW_HEIGHT / 2) - broslength, 20, DARKPURPLE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
