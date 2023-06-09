#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include "raygui.h"
#include "rlgl.h"
#include "screen.h"
#include "sim.h"
#include <stdio.h>
#include <strings.h>

extern char textInput[50];
extern char textInputFileName[50];
extern bool showSaveWindow;
extern bool showLoadWindow;
extern bool inUIRegion;
extern Rectangle colorsRects[4];

void drawCursor(Vector2 pos);
void savePopUp(void);
void loadPopUp(void);
int drawHelpItem(cell_state state, cell_state selected, int x, int y);
void drawPlayingOrPausedIndicator();
void drawSpeed();
void draw2Dgrid(void);
void drawSelectedCell(cell_coord selected_cell, cell_state state);
void drawCells(cell_coord *changedCoords, size_t num_changed_coords);
void drawBorder(float zoomlevel);
void drawUIBackground(int UIBarHeight);

#endif // !GUI_UTILS_H
