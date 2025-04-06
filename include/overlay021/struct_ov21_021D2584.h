#ifndef POKEPLATINUM_STRUCT_OV21_021D2584_H
#define POKEPLATINUM_STRUCT_OV21_021D2584_H

#include "sprite.h"
#include "sprite_resource.h"

typedef struct {
    Sprite *sprite[4];
    SpriteResource *spriteResource[4];
    int x;
    int y;
    int width;
    int height;
    BOOL staticSize;
    int scale;
} CursorGraphics;

#endif // POKEPLATINUM_STRUCT_OV21_021D2584_H
