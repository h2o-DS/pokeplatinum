#ifndef POKEPLATINUM_POKEDEX_TEXT_MANAGER_H
#define POKEPLATINUM_POKEDEX_TEXT_MANAGER_H

#include "struct_decls/font_oam.h"

#include "char_transfer.h"

typedef struct {
    FontOAM *fontOAM;
    CharTransferAllocation charTransferAllocation;
} PokedexTextManager;

#endif // POKEPLATINUM_POKEDEX_TEXT_MANAGER_H
