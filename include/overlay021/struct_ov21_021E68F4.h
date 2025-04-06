#ifndef POKEPLATINUM_STRUCT_OV21_021E68F4_H
#define POKEPLATINUM_STRUCT_OV21_021E68F4_H

#include "overlay021/funcptr_ov21_021D41EC.h"
#include "overlay021/funcptr_ov21_021D420C.h"
#include "overlay021/struct_ov21_021D4660.h"

enum PokedexTransitionMode {
    POKEDEX_TRANSITION_IN = 0,
    POKEDEX_TRANSITION_UPDATE,
    POKEDEX_TRANSITION_OUT,
    MAX_POKEDEX_TRANSITION_MODE
};

typedef struct {
    void *operationalData;
    void *graphicData;
    PokedexDataFunction dataTransition[MAX_POKEDEX_TRANSITION_MODE];
    PokedexGraphicsFunction displayTransition[MAX_POKEDEX_TRANSITION_MODE];
    PokedexScreenState *pokedexScreenState;
    int numScreenStates;
} PokedexScreenManager;

#endif // POKEPLATINUM_STRUCT_OV21_021E68F4_H
