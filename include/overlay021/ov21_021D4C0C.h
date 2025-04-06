#ifndef POKEPLATINUM_OV21_021D4C0C_H
#define POKEPLATINUM_OV21_021D4C0C_H

#include "overlay021/pokedex_display_box.h"
#include "overlay021/pokedex_text_manager.h"
#include "overlay021/struct_ov21_021D1FA4.h"
#include "overlay021/struct_ov21_021D4C0C_decl.h"

#include "bg_window.h"
#include "strbuf.h"

UnkStruct_ov21_021D4C0C *ov21_021D4C0C(const UnkStruct_ov21_021D1FA4 *param0);
void ov21_021D4C6C(UnkStruct_ov21_021D4C0C *param0);
PokedexTextManager *ov21_021D4CA0(const PokedexDisplayBox *param0);
PokedexTextManager *ov21_021D4CB8(const PokedexDisplayBox *param0, int param1);
void ov21_021D4D1C(PokedexTextManager *param0);
void ov21_021D4D3C(UnkStruct_ov21_021D4C0C *param0);
Window *Pokedex_NewWindow(UnkStruct_ov21_021D4C0C *param0, int width, int height);
void Pokedex_FreeWindow(Window *window);
u32 Pokedex_DisplayMessage(UnkStruct_ov21_021D4C0C *param0, Window *window, u32 bankID, u32 entryID, int xOffset, int yOffset);
void Pokedex_DisplayMessageBottomCentered(UnkStruct_ov21_021D4C0C *param0, Window *window, u32 bankID, u32 entryID);
void Pokedex_DisplayText(UnkStruct_ov21_021D4C0C *param0, Window *window, Strbuf *strbuf, int xOffset, int yOffset);

#endif // POKEPLATINUM_OV21_021D4C0C_H
