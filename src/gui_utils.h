#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include "raygui.h"
#include "raylib.h"
#include "sim.h"
#include <stdio.h>
#include <strings.h>

#define CELL_SIZE 20
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

char textInput[50] = {0};
char textInputFileName[50] = {0};
bool showSaveWindow = false;
bool showLoadWindow = false;
Rectangle colorsRects[4] = {0};

void drawCursor(Vector2 pos);
void savePopUp(void);
void loadPopUp(void);
int drawHelpItem(cell_state state, cell_state selected, int x, int y);
void drawPlayingOrPausedIndicator();
void drawSpeed();
void draw2Dgrid(void);
void drawSelectedCell(cell_coord selected_cell, cell_state state);
void drawCells();

#endif // !GUI_UTILS_H
