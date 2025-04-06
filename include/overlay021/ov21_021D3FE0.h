#ifndef POKEPLATINUM_OV21_021D3FE0_H
#define POKEPLATINUM_OV21_021D3FE0_H

#include "overlay021/struct_ov21_021D3FE0_decl.h"
#include "overlay021/struct_ov21_021D4008.h"

#include "heap.h"

UnkStruct_ov21_021D3FE0 *PokedexUpdater_InitUpdater(enum HeapId heapID);
void PokedexUpdater_PopulateUpdater(UnkStruct_ov21_021D3FE0 *param0, const UnkStruct_ov21_021D4008 *param1);
int PokedexUpdater_UpdateData(UnkStruct_ov21_021D3FE0 *param0);
int PokedexUpdater_UpdateGraphics(UnkStruct_ov21_021D3FE0 *param0);
void PokedexUpdater_SetDataUnchanged(UnkStruct_ov21_021D3FE0 *param0, BOOL param1);
void PokedexUpdater_DataExit(UnkStruct_ov21_021D3FE0 *param0);
BOOL ov21_021D41A8(UnkStruct_ov21_021D3FE0 *param0);
BOOL ov21_021D41C0(UnkStruct_ov21_021D3FE0 *param0);

#endif // POKEPLATINUM_OV21_021D3FE0_H
