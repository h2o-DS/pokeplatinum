#ifndef POKEPLATINUM_STRUCT_OV21_021E6B20_H
#define POKEPLATINUM_STRUCT_OV21_021E6B20_H

#include "heap.h"

typedef struct {
    int state;
    enum HeapId heapID;
    void *pageGraphics;
} PokedexGraphicsManager;

#endif // POKEPLATINUM_STRUCT_OV21_021E6B20_H
