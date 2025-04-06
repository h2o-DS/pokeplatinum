#ifndef POKEPLATINUM_STRUCT_OV21_021E6A68_H
#define POKEPLATINUM_STRUCT_OV21_021E6A68_H

#include "constants/heap.h"

typedef struct {
    int state;
    enum HeapId heapID;
    void *pageData;
    BOOL exit;
    BOOL unchanged;
} PokedexDataManager;

#endif // POKEPLATINUM_STRUCT_OV21_021E6A68_H
