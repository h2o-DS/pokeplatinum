#ifndef POKEPLATINUM_POKEDEX_APP_H
#define POKEPLATINUM_POKEDEX_APP_H

#include "overlay021/pokedex_graphic_data.h"
#include "overlay021/pokedex_sort_data.h"
#include "overlay021/struct_ov21_021D3FE0_decl.h"
#include "overlay021/struct_ov21_021D423C_decl.h"
#include "overlay021/struct_ov21_021D4660.h"
#include "overlay021/struct_ov21_021E68F4.h"

#include "pokedex_memory.h"

enum PokedexTopScreen {
    POKEDEX_TOP_EXIT = 0,
    POKEDEX_TOP_FADE,
    POKEDEX_TOP_INFO,
    POKEDEX_TOP_FORMS,
    POKEDEX_TOP_FOREIGN, // foreign info page
    POKEDEX_TOP_CLEAR,
    POKEDEX_TOP_AREA,
    POKEDEX_TOP_CRY,
    POKEDEX_TOP_HEIGHTCHECK,
    POKEDEX_TOP_WEIGHTCHECK,
    MAX_POKEDEX_TOP_SCREEN
};

enum PokedexBottomScreen {
    POKEDEX_BOTTOM_EXIT = 0,
    POKEDEX_BOTTOM_FADE,
    POKEDEX_BOTTOM_INFO,
    POKEDEX_BOTTOM_CLEAR,
    POKEDEX_BOTTOM_AREA,
    POKEDEX_BOTTOM_CRY,
    POKEDEX_BOTTOM_SIZE,
    POKEDEX_BOTTOM_FORMS,
    MAX_POKEDEX_BOTTOM_SCREEN
};

typedef struct PokedexApp {
    BOOL unk_00;
    int heapID;
    PokedexMemory *pokedexMemory;
    PokedexSortData sortData;
    PokedexGraphicData graphicData;
    int unk_1A60;
    int unk_1A64;
    int pageBinary;
    int unk_1A6C;
    UnkStruct_ov21_021D423C *unk_1A70;
    UnkStruct_ov21_021D423C *unk_1A74;
    UnkStruct_ov21_021D423C *unk_1A78;
    UnkStruct_ov21_021D423C *unk_1A7C;
    PokedexScreenState unk_1A80;
    PokedexScreenManager topScreen[MAX_POKEDEX_TOP_SCREEN];
    PokedexScreenManager bottomScreen[MAX_POKEDEX_BOTTOM_SCREEN];
    UnkStruct_ov21_021D3FE0 *unk_1D64;
    UnkStruct_ov21_021D3FE0 *unk_1D68;
    UnkStruct_ov21_021D3FE0 *unk_1D6C;
    UnkStruct_ov21_021D3FE0 *unk_1D70;
} PokedexApp;

#endif // POKEPLATINUM_POKEDEX_APP_H
