#ifndef POKEPLATINUM_OV21_021E29DC_H
#define POKEPLATINUM_OV21_021E29DC_H

#include "overlay021/pokedex_app.h"
#include "overlay021/struct_ov21_021E68F4.h"

#include "heap.h"

enum PokedexButtonStatus {
    POKEDEX_BUTTON_DEFAULT = 0,
    POKEDEX_BUTTON_ACTIVE,
    POKEDEX_BUTTON_PRESSED,
    POKEDEX_BUTTON_DEACTIVE,
    POKEDEX_BUTTON_INIT
};

void PokedexEntry_BannerTransitions(PokedexScreenManager *param0, PokedexApp *param1, enum HeapId heapID);
void PokedexEntry_FreeData(PokedexScreenManager *param0);
BOOL ov21_021E2A54(const PokedexScreenManager *param0);
void PokedexEntry_PanelTransitions(PokedexScreenManager *param0, PokedexApp *param1, enum HeapId heapID);
void PokedexEntry_FreeTransitionData(PokedexScreenManager *param0);
BOOL ov21_021E33A4(const PokedexScreenManager *param0);
BOOL ov21_021E33AC(const PokedexScreenManager *param0);
void ov21_021E33B4(PokedexScreenManager *param0, BOOL param1);
BOOL ov21_021E33BC(const PokedexScreenManager *param0);

#endif // POKEPLATINUM_OV21_021E29DC_H
