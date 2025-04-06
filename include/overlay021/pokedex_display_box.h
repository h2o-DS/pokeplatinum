#ifndef POKEPLATINUM_POKEDEX_DISPLAY_BOX_H
#define POKEPLATINUM_POKEDEX_DISPLAY_BOX_H

#include <nnsys.h>

#include "overlay021/struct_ov21_021D4C0C_decl.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"

typedef struct {
    UnkStruct_ov21_021D4C0C *unk_00;
    const Window *window;
    const NNSG2dImagePaletteProxy *paletteProxy;
    const Sprite *sprite;
    int x;
    int y;
    int unk_18;
    int unk_1C;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapId heapID;
} PokedexDisplayBox;

#endif // POKEPLATINUM_POKEDEX_DISPLAY_BOX_H
