#import "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
