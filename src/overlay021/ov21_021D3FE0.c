#include <nitro.h>
#include <string.h>

#include "overlay021/PokedexUpdater_InitUpdater.h"
#include "overlay021/funcptr_ov21_021D41EC.h"
#include "overlay021/funcptr_ov21_021D420C.h"
#include "overlay021/struct_ov21_021D4008.h"
#include "overlay021/struct_ov21_021E6A68.h"
#include "overlay021/struct_ov21_021E6B20.h"

#include "heap.h"

typedef struct UnkStruct_ov21_021D3FE0_t {
    void *pageData;
    void *pageGraphics;
    int state;
    PokedexDataManager dataMan;
    PokedexDataFunction dataFunc[3];
    PokedexGraphicsManager graphicsMan;
    PokedexGraphicsFunction graphicsFunc[3];
    enum HeapId heapID;
} PokedexUpdater;

static void ResetPageMan(PokedexDataManager *dataMan);
static void ResetGraphicsMan(PokedexGraphicsManager *graphicsMan);
static int RunDataFunction(PokedexDataFunction dataFunc, PokedexDataManager *dataMan, void *param2);
static int RunGraphicsFunction(PokedexGraphicsFunction graphicsFunc, void *param1, PokedexGraphicsManager *graphicsMan, const void *param3, const PokedexDataManager *dataMan);

PokedexUpdater *PokedexUpdater_InitUpdater(enum HeapId heapID)
{
    PokedexUpdater *pokedexUpdater;

    pokedexUpdater = Heap_AllocFromHeap(heapID, sizeof(PokedexUpdater));

    GF_ASSERT(pokedexUpdater);
    memset(pokedexUpdater, 0, sizeof(PokedexUpdater));

    pokedexUpdater->state = 5;
    pokedexUpdater->heapID = heapID;

    return pokedexUpdater;
}

void PokedexUpdater_PopulateUpdater(PokedexUpdater *pokedexUpdater, const UnkStruct_ov21_021D4008 *param1)
{
    GF_ASSERT(pokedexUpdater);
    GF_ASSERT(param1);
    GF_ASSERT(ov21_021D41A8(pokedexUpdater));

    pokedexUpdater->pageData = param1->unk_00;
    pokedexUpdater->pageGraphics = param1->unk_04;
    pokedexUpdater->state = 0;

    ResetPageMan(&pokedexUpdater->dataMan);
    pokedexUpdater->dataMan.heapID = pokedexUpdater->heapID;

    ResetGraphicsMan(&pokedexUpdater->graphicsMan);
    pokedexUpdater->graphicsMan.heapID = pokedexUpdater->heapID;

    pokedexUpdater->dataFunc[0] = param1->unk_08[0];
    pokedexUpdater->dataFunc[1] = param1->unk_08[1];
    pokedexUpdater->dataFunc[2] = param1->unk_08[2];
    pokedexUpdater->graphicsFunc[0] = param1->unk_0C[0];
    pokedexUpdater->graphicsFunc[1] = param1->unk_0C[1];
    pokedexUpdater->graphicsFunc[2] = param1->unk_0C[2];
}

int PokedexUpdater_UpdateData(PokedexUpdater *pokedexUpdater)
{
    int v0;

    GF_ASSERT(pokedexUpdater);

    switch (pokedexUpdater->state) {
    case 0:
        v0 = RunDataFunction(pokedexUpdater->dataFunc[0], &pokedexUpdater->dataMan, pokedexUpdater->pageData);

        if (v0 == 1) {
            pokedexUpdater->state++;
            pokedexUpdater->dataMan.state = 0;
        }
        break;
    case 1:
        break;
    case 2:
        v0 = RunDataFunction(pokedexUpdater->dataFunc[1], &pokedexUpdater->dataMan, pokedexUpdater->pageData);

        if (v0 == 1) {
            pokedexUpdater->state++;
            pokedexUpdater->dataMan.state = 0;
            pokedexUpdater->graphicsMan.state = 0;
        }
        break;
    case 3:
        break;
    case 4:
        v0 = RunDataFunction(pokedexUpdater->dataFunc[2], &pokedexUpdater->dataMan, pokedexUpdater->pageData);

        if (v0 == 1) {
            pokedexUpdater->state++;
        }
        break;
    case 5:
        return 1;
    default:
        GF_ASSERT(0);
        break;
    }

    return 0;
}

int PokedexUpdater_UpdateGraphics(PokedexUpdater *pokedexUpdater)
{
    int v0;

    GF_ASSERT(pokedexUpdater);

    switch (pokedexUpdater->state) {
    case 0:
        break;
    case 1:
        v0 = RunGraphicsFunction(pokedexUpdater->graphicsFunc[0], pokedexUpdater->pageGraphics, &pokedexUpdater->graphicsMan, pokedexUpdater->pageData, &pokedexUpdater->dataMan);

        if (v0 == 1) {
            pokedexUpdater->state++;
            pokedexUpdater->graphicsMan.state = 0;
        }
        break;
    case 2:
        RunGraphicsFunction(pokedexUpdater->graphicsFunc[1], pokedexUpdater->pageGraphics, &pokedexUpdater->graphicsMan, pokedexUpdater->pageData, &pokedexUpdater->dataMan);
        break;
    case 3:
        v0 = RunGraphicsFunction(pokedexUpdater->graphicsFunc[2], pokedexUpdater->pageGraphics, &pokedexUpdater->graphicsMan, pokedexUpdater->pageData, &pokedexUpdater->dataMan);

        if (v0 == 1) {
            pokedexUpdater->state++;
        }
        break;
    case 4:
        break;
    case 5:
        return 1;
    default:
        GF_ASSERT(0);
        break;
    }

    return 0;
}

void PokedexUpdater_SetDataUnchanged(PokedexUpdater *pokedexUpdater, BOOL unchanged)
{
    pokedexUpdater->dataMan.unchanged = unchanged;
}

void PokedexUpdater_DataExit(PokedexUpdater *pokedexUpdater)
{
    GF_ASSERT(pokedexUpdater);
    pokedexUpdater->dataMan.exit = TRUE;
}

BOOL ov21_021D41A8(PokedexUpdater *pokedexUpdater)
{
    GF_ASSERT(pokedexUpdater);

    if (pokedexUpdater->state == 5) {
        return 1;
    }

    return 0;
}

BOOL ov21_021D41C0(PokedexUpdater *pokedexUpdater)
{
    GF_ASSERT(pokedexUpdater);

    if (pokedexUpdater->state == 2) {
        return 1;
    }

    return 0;
}

static void ResetPageMan(PokedexDataManager *dataMan)
{
    dataMan->state = 0;
    dataMan->pageData = NULL;
    dataMan->exit = FALSE;
}

static void ResetGraphicsMan(PokedexGraphicsManager *graphicsMan)
{
    graphicsMan->state = 0;
    graphicsMan->pageGraphics = NULL;
}

static int RunDataFunction(PokedexDataFunction dataFunc, PokedexDataManager *dataMan, void *param2)
{
    int v0;

    GF_ASSERT(dataFunc);
    GF_ASSERT(dataMan);

    v0 = dataFunc(dataMan, param2);

    return v0;
}

static int RunGraphicsFunction(PokedexGraphicsFunction graphicsFunc, void *param1, PokedexGraphicsManager *graphicsMan, const void *param3, const PokedexDataManager *dataMan)
{
    int v0;

    GF_ASSERT(graphicsFunc);
    GF_ASSERT(graphicsMan);
    GF_ASSERT(dataMan);

    v0 = graphicsFunc(param1, graphicsMan, param3, dataMan);

    return v0;
}
