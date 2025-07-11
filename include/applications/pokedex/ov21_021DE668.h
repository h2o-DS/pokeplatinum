#ifndef POKEPLATINUM_OV21_021DE668_H
#define POKEPLATINUM_OV21_021DE668_H

#include "applications/pokedex/pokedex_app.h"
#include "applications/pokedex/pokedex_text_manager.h"
#include "applications/pokedex/struct_ov21_021DE6D4.h"
#include "applications/pokedex/struct_ov21_021E68F4.h"

#include "bg_window.h"
#include "heap.h"

void ov21_021DE668(UnkStruct_ov21_021E68F4 *param0, PokedexApp *param1, enum HeapId heapID);
void ov21_021DE6C0(UnkStruct_ov21_021E68F4 *param0);
BOOL ov21_021DE6D4(UnkStruct_ov21_021DE6D4 *param0, int param1);
BOOL ov21_021DE6D8(UnkStruct_ov21_021DE6D4 *param0, int param1);
void ov21_021DEB8C(Window *param0, int param1, enum HeapId heapID, int param3, u32 param4);
int PokedexGraphics_GetAnimIDfromType(int monType);
Window *ov21_021DF30C(PokedexTextManager *textMan, int param1, int param2);

#endif // POKEPLATINUM_OV21_021DE668_H
