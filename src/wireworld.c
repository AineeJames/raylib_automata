#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "styles/style_dark.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * If much larger cell grid is desired
 * may need to improve rectangle grid rendering
 * for example with cell_size 5 we have 160x78
 * which is 12480 calls to drawRectangle
 * Instead maybe writing to a texture may
 * be more efficient?
 * */

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 20
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

// const Color COPPER_PASTEL = {221, 140, 88, 255};
Color state_colors[] = {(Color){221, 140, 88, 255}, BLUE, RED, BLACK};

const char *state_names[] = {"WIRE", "HEAD", "TAIL", "ERASE"};

typedef enum { WIRE, HEAD, TAIL, EMPTY } cell_state;

typedef struct {
  int x;
  int y;
} cell_coord;

cell_state cell_grid[GRID_WIDTH][GRID_HEIGHT];
cell_state next_cell_grid[GRID_WIDTH][GRID_HEIGHT];
bool playing = false;
int frames_per_tick = 4;
int frame_count = 0;
bool showSaveWindow = false;
bool showLoadWindow = false;
Rectangle colorsRects[4] = {0};
char textInput[50] = {0};
char textInputFileName[50] = {0};

void clearCells(void);
void draw2Dgrid(void);
void drawSelectedCell(cell_coord selected_cell, cell_state state);
void drawCells();
void updateGrid(void);
cell_coord getCellIdx(Vector2 mouse_pos);
void setCell(cell_coord coordinate, cell_state new_state);
int drawHelpItem(cell_state state, cell_state selected, int x, int y);
void drawPlayingOrPausedIndicator();
void drawSpeed();
int stateInMoore(int x, int y, cell_state target_state);
void drawCursor(Vector2 pos);
void savePopUp(void);
void loadPopUp(void);

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 50, "Wire World");
  GuiLoadStyleDark();
  SetTargetFPS(60);
  HideCursor();
  clearCells();
  cell_state draw_state = WIRE;
  int stateMouseHover = 0;
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
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < 4; i++) {
      if (CheckCollisionPointRec(mousePos, colorsRects[i])) {
        stateMouseHover = i;
        break;
      } else
        stateMouseHover = -1;
    }

    cell_coord selected_cell = getCellIdx(mousePos);
    if ((stateMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      draw_state = stateMouseHover;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !showSaveWindow &&
        !showLoadWindow)
      setCell(selected_cell, draw_state);

    BeginDrawing();
    ClearBackground(BLACK);
    drawCells();
    draw2Dgrid();
    DrawFPS(0, 0);
    int offset = 15;
    for (int state = WIRE; state < EMPTY + 1; state++) {
      offset +=
          drawHelpItem(state, draw_state, offset, WINDOW_HEIGHT + 15) + 20;
    }
    drawSpeed();
    DrawText("x: Clear Screen", offset, WINDOW_HEIGHT + 15, 20, RAYWHITE);
    drawPlayingOrPausedIndicator();
    drawSelectedCell(selected_cell, draw_state);
    if (showSaveWindow) {
      savePopUp();
    }

    if (showLoadWindow) {
      loadPopUp();
    }

    drawCursor(mousePos);
    EndDrawing();

    memcpy(&next_cell_grid, &cell_grid,
           GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
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

void drawCursor(Vector2 pos) {
  DrawRectangleLines(pos.x - 5, pos.y - 5, 8, 8, RAYWHITE);
  DrawRectangle(pos.x - 2, pos.y - 2, 2, 2, RAYWHITE);
}

void clearCells(void) {
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      cell_grid[i][j] = EMPTY;
    }
  }
}

void drawSpeed() {
  char speedstr[50];
  sprintf(speedstr, "%d", frames_per_tick);
  DrawText(speedstr, 600, 620, 20, GREEN);
}

void drawPlayingOrPausedIndicator() {
  Vector2 x1 = {780, 620};
  Vector2 x2 = {760, 610};
  Vector2 x3 = {760, 630};
  if (playing) {
    DrawText("Playing", 680, 620, 20, GREEN);
    DrawTriangle(x1, x2, x3, GREEN);
  } else {
    DrawText("Paused", 680, 620, 20, RED);
    DrawTriangle(x1, x2, x3, RED);
  }
}

int drawHelpItem(cell_state state, cell_state selected, int x, int y) {
  char label[50];
  sprintf(label, "%d: %s", state + 1, state_names[state]);
  int width = MeasureText(label, 20);
  Color rect_color =
      state < EMPTY ? state_colors[state] : (Color){75, 75, 75, 255};
  DrawRectangle(x - 2, y, MeasureText(label, 20) + 4, 20, rect_color);

  colorsRects[state] = (Rectangle){x - 2, y, MeasureText(label, 20) + 4, 20};

  if (state == selected) {
    DrawRectangleLines(x - 5, y - 3, MeasureText(label, 20) + 10, 26, WHITE);
  }
  DrawText(label, x, y, 20, BLACK);

  return width;
}

void setCell(cell_coord coordinate, cell_state new_state) {
  if (coordinate.x >= 0 && coordinate.y >= 0 && coordinate.x < GRID_WIDTH &&
      coordinate.y < GRID_HEIGHT) {
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

void drawSelectedCell(cell_coord selected_cell, cell_state state) {

  if (selected_cell.x >= 0 && selected_cell.y >= 0 &&
      selected_cell.x < GRID_WIDTH && selected_cell.y < GRID_HEIGHT) {
    DrawRectangle(selected_cell.x * CELL_SIZE, selected_cell.y * CELL_SIZE,
                  CELL_SIZE, CELL_SIZE, Fade(state_colors[state], 0.75f));
    DrawRectangleLines(selected_cell.x * CELL_SIZE, selected_cell.y * CELL_SIZE,
                       CELL_SIZE, CELL_SIZE, WHITE);
  }
}

void drawCells(void) {
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                    state_colors[cell_grid[i][j]]);
    }
  }
}

void draw2Dgrid(void) {
  Color Griddycolor = {50, 50, 50, 255};
  for (int i = 0; i <= WINDOW_WIDTH; i += CELL_SIZE) {
    // Vertical Lines
    Vector2 linestart = {i, 0};
    Vector2 lineend = {i, WINDOW_HEIGHT};
    DrawLineEx(linestart, lineend, 1, Griddycolor);
    for (int j = 0; j <= WINDOW_HEIGHT; j += CELL_SIZE) {
      // Horizontal lines
      Vector2 linestart = {0, j};
      Vector2 lineend = {WINDOW_WIDTH, j};
      DrawLineEx(linestart, lineend, 1, Griddycolor);
    }
  }
}
int stateInMoore(int x, int y, cell_state target_state) {
  int count = 0;

  // Check all 8 neighboring cells for the target state
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      // Skip the cell itself
      if (i == x && j == y)
        continue;

      // Check if the neighboring cell is in bounds
      if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {
        // Check if the neighboring cell has the target state
        if (cell_grid[i][j] == target_state) {
          count++;
        }
      }
    }
  }

  return count;
}

void updateGrid(void) {
  /*
   * As in all cellular automata, time proceeds in discrete steps called
   * generations (sometimes gens or ticks). Cells behave as follows:
   *
   *     empty → empty,
   *     electron head → electron tail,
   *     electron tail → conductor,
   *     conductor → electron head if exactly one or two of the neighbouring
   * cells are electron heads, otherwise remains conductor.*/
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      if (cell_grid[i][j] == TAIL) {
        next_cell_grid[i][j] = WIRE;
      }
      if (cell_grid[i][j] == HEAD && stateInMoore(i, j, TAIL) > 0) {
        next_cell_grid[i][j] = TAIL;
      }
      int number_of_heads = stateInMoore(i, j, HEAD);
      if (cell_grid[i][j] == WIRE &&
          (number_of_heads == 1 || number_of_heads == 2)) {

        next_cell_grid[i][j] = HEAD;
      }
    }
  }
  memcpy(&cell_grid, &next_cell_grid,
         GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
}

void savePopUp(void) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                Fade(RAYWHITE, 0.8f));
  int result =
      GuiTextInputBox((Rectangle){(float)GetScreenWidth() / 2 - 120,
                                  (float)GetScreenHeight() / 2 - 60, 240, 140},
                      "Save", GuiIconText(ICON_FILE_SAVE, "Save file as..."),
                      "Ok;Cancel", textInput, 255, NULL);

  if (result == 1) {
    // TODO: Validate textInput value and save

    strcpy(textInputFileName, textInput);
    strcat(textInputFileName, ".wwld");
    SaveFileData(textInputFileName, cell_grid,
                 GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
  }

  if ((result == 0) || (result == 1) || (result == 2)) {
    strcpy(textInput, "\0");
    showSaveWindow = false;
  }
}

void loadPopUp(void) {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                Fade(RAYWHITE, 0.8f));
  int result =
      GuiTextInputBox((Rectangle){(float)GetScreenWidth() / 2 - 120,
                                  (float)GetScreenHeight() / 2 - 60, 240, 140},
                      "Load", GuiIconText(ICON_FILE_COPY, "Load file..."),
                      "Ok;Cancel", textInput, 255, NULL);

  if (result == 1) {
    // TODO: added file selection dialog for load exists
    // example here
    // https://github.com/raysan5/raygui/blob/master/examples/custom_file_dialog/gui_file_dialog.h

    strcpy(textInputFileName, textInput);
    bool real_file = FileExists(textInputFileName);
    if (real_file) {
      TraceLog(LOG_DEBUG, "Attempting to load from file %s\n",
               textInputFileName);
      unsigned int bytes_read;
      unsigned char *loaded_data = LoadFileData(textInputFileName, &bytes_read);
      TraceLog(LOG_INFO, "Read %d bytes from file %s\n", bytes_read,
               textInputFileName);
      unsigned int expected_bytes =
          GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state);

      if (bytes_read == expected_bytes) {
        // TODO validate loaded cell_grid to
        // ensure the values make sense i.e. are
        // in enum
        memcpy(cell_grid, loaded_data,
               GRID_WIDTH * GRID_HEIGHT * sizeof(cell_state));
      } else {
        // TODO Add popup when load fails
        TraceLog(LOG_ERROR, "File %s loaded %d bytes when %d were expected\n",
                 textInputFileName, bytes_read, expected_bytes);
      }

    } else {
      TraceLog(LOG_DEBUG, "tried to load from %s but found it was not a file\n",
               textInputFileName);
    }
  }

  if ((result == 0) || (result == 1) || (result == 2)) {
    strcpy(textInput, "\0");
    showLoadWindow = false;
  }
}