#ifndef POKEPLATINUM_OV21_021D4EE4_H
#define POKEPLATINUM_OV21_021D4EE4_H

#include "constants/heap.h"

enum PanelMovementType {
    PANEL_MOVEMENT_MULTICOL = 0,
    PANEL_MOVEMENT_MULTIROW,
    PANEL_MOVEMENT_COL,
    PANEL_MOVEMENT_ROW,
    PANEL_MOVEMENT_JUMP
};

enum CursorMovementType {
    CURSOR_MOVEMENT_SELECT = 0,
    CURSOR_MOVEMENT_IMPASSE,
    CURSOR_MOVEMENT_SKIP
};

typedef struct PokedexCursorData {
    int x;
    int y;
    int width;
    int height;
    enum CursorMovementType cursorColMovementType;
    enum CursorMovementType cursorRowMovementType;
    int button;
} PokedexCursorData;

typedef struct PokedexPanelData {
    const PokedexCursorData *pokedexCursorData;
    int numCols;
    int numRows;
    int col;
    int row;
} PokedexPanelData;

PokedexPanelData *PokedexPanel_Alloc(enum HeapId heapID);
void PokedexPanel_Free(PokedexPanelData *pokedexPanelData);
void PokedexPanel_InitPokedexPanelData(PokedexPanelData *pokedexPanelData, const PokedexCursorData *pokedexCursorData, int numCols, int numRows);
void PokedexPanel_Move(PokedexPanelData *pokedexPanelData, enum PanelMovementType movementType, int val);
int PokedexPanel_GetCurrentButton(const PokedexPanelData *pokedexPanelData);
void PokedexPanel_GetCurrentButtonXY(const PokedexPanelData *pokedexPanelData, int *x, int *y);
void PokedexPanel_GetCurrentButtonWidthHeight(const PokedexPanelData *pokedexPanelData, int *width, int *height);
void PokedexPanel_InitPokedexCursorData(PokedexCursorData *pokedexCursorData, int x, int y, int width, int height, enum CursorMovementType cursorColMovementType, enum CursorMovementType cursorRowMovementType, int button);
void PokedexPanel_GetCursorCornerPos(int cornerIndex, int *cornerX, int *cornerY, int x, int y, int width, int height);

#endif // POKEPLATINUM_OV21_021D4EE4_H
