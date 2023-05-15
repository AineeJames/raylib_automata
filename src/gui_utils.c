#include "gui_utils.h"
char textInput[50] = {0};
char textInputFileName[50] = {0};
bool showSaveWindow = false;
bool showLoadWindow = false;
bool inUIRegion = false;
Rectangle colorsRects[4] = {0};

void drawCursor(Vector2 pos) {
  DrawRectangleLines(pos.x - 5, pos.y - 5, 8, 8, RAYWHITE);
  DrawRectangle(pos.x - 2, pos.y - 2, 2, 2, RAYWHITE);
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

void drawUIBackground(int UIBarHeight){
    DrawRectangle(0,WINDOW_HEIGHT,WINDOW_WIDTH, 50, BLACK);
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

void drawSpeed() {
  char speedstr[50];
  sprintf(speedstr, "%d", frames_per_tick);
  DrawText(speedstr, 600, 620, 20, GREEN);
}

void draw2Dgrid(void) {
    rlPushMatrix();
    rlTranslatef((GRID_WIDTH * CELL_SIZE)/2.0, (GRID_HEIGHT * CELL_SIZE)/2.0, 0);
    rlRotatef(90, 1, 0, 0);
    DrawGrid(GRID_WIDTH, CELL_SIZE);
    rlPopMatrix();

    rlPushMatrix();
    rlTranslatef((GRID_WIDTH * CELL_SIZE)/2.0, (GRID_HEIGHT * CELL_SIZE), 0);
    rlRotatef(90, 1, 0, 0);
    DrawGrid(GRID_WIDTH, CELL_SIZE);
    rlPopMatrix();
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

void drawCells(cell_coord *changedCoords, size_t num_changed_coords) {

  for (int i = 0; i < num_changed_coords; i++){
	cell_coord cur = changedCoords[i];
	DrawPixel(cur.x,cur.y,state_colors[cell_grid[cur.x][cur.y]]);
  }
}

void drawBorder(float zoomlevel) {
  Rectangle rect = {0,0, GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE};
  DrawRectangleLinesEx(rect,3.0f * (1/zoomlevel),WHITE);
}
