#ifndef POKEPLATINUM_STRUCT_OV21_021D3124_H
#define POKEPLATINUM_STRUCT_OV21_021D3124_H

#include "overlay021/struct_ov21_021D2584.h"
#include "overlay021/struct_ov21_021D2648.h"

typedef struct {
    CursorGraphics *cursorGraphics;
    CursorTransformation centerPoint;
    CursorTransformation size;
    int x;
    int y;
    int width;
    int height;
    BOOL moving;
} CursorManager;

#endif // POKEPLATINUM_STRUCT_OV21_021D3124_H
