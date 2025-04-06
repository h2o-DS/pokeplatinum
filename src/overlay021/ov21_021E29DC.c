#include "overlay021/ov21_021E29DC.h"

#include <nitro.h>
#include <string.h>

#include "generated/sdat.h"
#include "generated/text_banks.h"

#include "struct_decls/struct_02023FCC_decl.h"

#include "overlay021/ov21_021D1FA4.h"
#include "overlay021/ov21_021D4340.h"
#include "overlay021/ov21_021D4C0C.h"
#include "overlay021/ov21_021DC9BC.h"
#include "overlay021/ov21_021E0C68.h"
#include "overlay021/ov21_021E3FFC.h"
#include "overlay021/pokedex_app.h"
#include "overlay021/pokedex_display_box.h"
#include "overlay021/pokedex_graphic_data.h"
#include "overlay021/pokedex_main.h"
#include "overlay021/pokedex_panel.h"
#include "overlay021/pokedex_sort.h"
#include "overlay021/pokedex_sort_data.h"
#include "overlay021/pokedex_text_manager.h"
#include "overlay021/struct_ov21_021D4660.h"
#include "overlay021/struct_ov21_021D5B68.h"
#include "overlay021/struct_ov21_021DE6D4.h"
#include "overlay021/struct_ov21_021E68F4.h"
#include "overlay021/struct_ov21_021E6A68.h"
#include "overlay021/struct_ov21_021E6B20.h"

#include "bg_window.h"
#include "brightness_controller.h"
#include "heap.h"
#include "narc.h"
#include "pltt_transfer.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_transfer.h"
#include "sprite_util.h"
#include "system.h"
#include "touch_screen.h"
#include "unk_02005474.h"
#include "unk_02012744.h"
#include "unk_02023FCC.h"

#include "res/text/bank/pokedex.h"

// button constants
#define BUTTON_HEIGHT        16
#define BUTTON_WIDTH         24
#define BUTTON_HITBOX_HEIGHT 32
#define BUTTON_HITBOX_WIDTH  40
#define BUTTON_Y             24
#define BUTTON_INFO_X        28
#define BUTTON_AREA_X        (BUTTON_INFO_X + BUTTON_HITBOX_WIDTH) // 68
#define BUTTON_CRY_X         (BUTTON_AREA_X + BUTTON_HITBOX_WIDTH) // 108
#define BUTTON_SIZE_X        (BUTTON_CRY_X + BUTTON_HITBOX_WIDTH) // 148
#define BUTTON_FORMS_X       (BUTTON_SIZE_X + BUTTON_HITBOX_WIDTH) // 188
#define BUTTON_EXIT_X        (BUTTON_FORMS_X + BUTTON_HITBOX_WIDTH) // 228
// bitwise page constants
#define SCREEN_SCROLL (0x1 << 1)
#define SCREEN_INFO   (0x1 << 2)
#define SCREEN_AREA   (0x1 << 3)
#define SCREEN_CRY    (0x1 << 4)
#define SCREEN_SIZE   (0x1 << 5)
#define SCREEN_FORMS  (0x1 << 8)

enum PokedexButtons {
    POKEDEX_BUTTONS_INFO = 0,
    POKEDEX_BUTTONS_AREA,
    POKEDEX_BUTTONS_CRY,
    POKEDEX_BUTTONS_SIZE,
    POKEDEX_BUTTONS_FORMS,
    POKEDEX_BUTTONS_EXIT,
    MAX_POKEDEX_BUTTONS
};

enum PokedexPages {
    POKEDEX_PAGES_INFO = 0,
    POKEDEX_PAGES_AREA,
    POKEDEX_PAGES_CRY,
    POKEDEX_PAGES_SIZE,
    POKEDEX_PAGES_FORMS,
    MAX_POKEDEX_PAGES
};

enum PokedexEntryStates {
    POKEDEX_BANNER_STATES_EXIT = 0,
    POKEDEX_BANNER_STATES_INFO,
    POKEDEX_BANNER_STATES_AREA,
    POKEDEX_BANNER_STATES_CRY,
    POKEDEX_BANNER_STATES_SIZE,
    POKEDEX_BANNER_STATES_HEIGHT,
    POKEDEX_BANNER_STATES_WEIGHT,
    POKEDEX_BANNER_STATES_FORMS,
    MAX_POKEDEX_BANNER_STATES
};

typedef struct BannerSettings {
    int *pageBinary;
    PokedexSortData *pokedexSortData;
    enum PokedexPages page;
    UnkStruct_ov21_021D5B68 *scrollData;
    UnkStruct_ov21_021DE6D4 *infoSprite;
    PokedexScreenManager *formScreen;
    PokedexScreenManager *foreignScreen;
    PokedexScreenManager *areaScreen;
    PokedexScreenManager *cryScreen;
    PokedexScreenManager *sizeScreen;
    BOOL unk_28;
    BOOL unk_2C;
    BOOL unk_30;
} BannerSettings;

typedef struct PageBannerData {
    enum PokedexPages page;
    PokedexTextManager *bannerTextMan[MAX_POKEDEX_PAGES];
    SpriteResource *spriteResource[4];
} PageBannerData;

typedef struct PageTransitionData {
    int *unk_00;
    BannerSettings *bannerSettings;
    PokedexSortData *sortData;
    int buttonStatus[MAX_POKEDEX_BUTTONS];
    BOOL bottomScreenFade;
    BOOL bottomScreenInteractible;
    BOOL exiting;
    BOOL cursorOnTarget;
} PageTransitionData;

typedef struct UnkStruct_ov21_021E37CC {
    void *unk_00;
    void *unk_04;
} UnkStruct_ov21_021E37CC;

typedef struct PokedexBottomData {
    UnkStruct_02023FCC *unk_00;
    TouchScreenHitTable *touchScreenHitTable;
    UnkStruct_ov21_021E37CC unk_08;
    int unk_10;
    PokedexPanelData *pokedexPanelData;
    PokedexCursorData *pokedexCursorData;
} PokedexBottomData;

typedef struct PageButtonGraphics {
    Sprite *sprite[MAX_POKEDEX_BUTTONS];
    SpriteResource *spriteResource[4];
} PageButtonGraphics;

static BannerSettings *NewBannerSettings(enum HeapId heapID, PokedexApp *pokedexApp);
static PokedexGraphicData **GetGraphicData1(enum HeapId heapID, PokedexApp *pokedexApp);
static PokedexScreenState *NewBannerScreenState(enum HeapId heapID, PokedexApp *pokedexApp);
static void FreeBannerSettings(BannerSettings *bannerSettings);
static void FreeGraphicData1(PokedexGraphicData **pokedexGraphicData);
static void FreeScreenState(PokedexScreenState *pokedexScreenState);
static int NumBannerStates(void);
static int ov21_021E2C24(PokedexDataManager *param0, void *param1);
static int ov21_021E2C2C(PokedexDataManager *param0, void *param1);
static int SetPageToInfo(PokedexDataManager *param0, void *param1);
static int EnterBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static int UpdateBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static int ExitBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static void ov21_021E3320(BannerSettings *bannerSettings);
static BOOL ReturnFalse(enum PokedexPages page);
static BOOL SetCurrentPage(BannerSettings *bannerSettings, enum PokedexPages newPage, enum PokedexPages oldPage);
static void ResetDisplay(BannerSettings *bannerSettings);
static void ExitEntry(BannerSettings *bannerSettings);
static void DisplayBanner(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, const BannerSettings *bannerSettings, int param3);
static void FreeBanner(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData);
static void InitTopScreenGraphics(PokedexGraphicData **pokedexGraphicData, const BannerSettings *bannerSettings, int param2);
static void SetPageBannerSprites(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int param2);
static void FreeBannerGraphics(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData);
static void dummy3(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int param2);
static void dummy4(PageBannerData *pageBannerData);
static void dummy5(PageBannerData *pageBannerData, int param1);
static void DisplayPageName(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int param2);
static void FreeBannerTextMan(PageBannerData *pageBannerData);
static void UpdateBannerDisplay(PageBannerData *pageBannerData, enum PokedexPages newPage, enum PokedexPages oldPage);
static void PageBannerXLU(PageBannerData *pageBannerData);
static void PageBannerNormal(PageBannerData *pageBannerData);
static PageTransitionData *NewPageTransitionData(enum HeapId heapID, PokedexApp *param1);
static PokedexGraphicData **GetGraphicData2(enum HeapId heapID, PokedexApp *param1);
static PokedexScreenState *NullScreenState(enum HeapId heapID, PokedexApp *param1);
static void FreePageTransitionData(PageTransitionData *pageTransitionData);
static void FreeGraphicData2(PokedexGraphicData **pokedexGraphicData);
static void dummy6(PokedexScreenState *pokedexScreenState);
static int NumPanelStates(void);
static int ResetBottomData(PokedexDataManager *param0, void *param1);
static int UpdateEntryPage(PokedexDataManager *param0, void *param1);
static int FreeBottomData(PokedexDataManager *param0, void *param1);
static int EnterPokedexEntry(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static int UpdateBottomDisplay(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static int ExitPokedexEntry(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3);
static BOOL GoToPage(PageTransitionData *pageTransitionData, enum PokedexPages newPage, enum PokedexPages oldPage);
static void InitHitTable(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData, enum HeapId heapID);
static void DeactivateAllButtons(PageTransitionData *pageTransitionData);
static void SetActiveButton(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData);
static void FreeHitTable(PokedexBottomData *pokedexBottomData);
static void ButtonEffects(u32 param0, u32 param1, void *param2);
static void InitCursorData(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData, enum HeapId heapID);
static void PressButton(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData);
static void freePanel(PokedexBottomData *pokedexBottomData);
static void CursorToActivePage(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData);
static void CheckForSpeciesChange(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData);
static void CursorToCurrentButton(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData, const PageTransitionData *pageTransitionData);
static void UpdateCursor(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData, const PageTransitionData *pageTransitionData);
static void CursorTransformToCurrent(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData);
static BOOL UpdateCursorVisibility(PokedexGraphicData **pokedexGraphicData, BOOL param1);
static void CheckForExit(PageTransitionData *pageTransitionData);
static void CheckForB(PageTransitionData *pageTransitionData);
static void setExitingFalse(PageTransitionData *pageTransitionData);
static void DisplayBottomScreen(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, const PageTransitionData *pageTransitionData, int heapID);
static void FreePageButtons(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData);
static void InitBottomScreenGraphics(PokedexGraphicData **pokedexGraphicData, const PageTransitionData *pageTransitionData, int heapID);
static void SetPageButtonSprites(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID);
static void FreePageButtonGraphics(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData);
static void DisplayPageButtons(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID);
static void RemovePageButtons(PageButtonGraphics *pageButtonGraphics);
static void DisplayFormButton(PageButtonGraphics *pageButtonGraphics, const PageTransitionData *pageTransitionData);
static void Dummy1(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID);
static void Dummy2(PageButtonGraphics *pageButtonGraphics);

void PokedexEntry_BannerTransitions(PokedexScreenManager *param0, PokedexApp *param1, enum HeapId heapID)
{
    BannerSettings *bannerSettings;
    PokedexGraphicData **pokedexGraphicData;
    PokedexScreenState *pokedexScreenState;

    bannerSettings = NewBannerSettings(heapID, param1);
    pokedexGraphicData = GetGraphicData1(heapID, param1);
    pokedexScreenState = NewBannerScreenState(heapID, param1);

    param0->operationalData = bannerSettings;
    param0->graphicData = pokedexGraphicData;
    param0->pokedexScreenState = pokedexScreenState;
    param0->numScreenStates = NumBannerStates();

    param0->dataTransition[POKEDEX_TRANSITION_IN] = ov21_021E2C24;
    param0->dataTransition[POKEDEX_TRANSITION_UPDATE] = ov21_021E2C2C;
    param0->dataTransition[POKEDEX_TRANSITION_OUT] = SetPageToInfo;
    param0->displayTransition[POKEDEX_TRANSITION_IN] = EnterBanner;
    param0->displayTransition[POKEDEX_TRANSITION_UPDATE] = UpdateBanner;
    param0->displayTransition[POKEDEX_TRANSITION_OUT] = ExitBanner;
}

void PokedexEntry_FreeData(PokedexScreenManager *param0)
{
    FreeBannerSettings(param0->operationalData);
    FreeGraphicData1(param0->graphicData);
    FreeScreenState(param0->pokedexScreenState);
}

BOOL ov21_021E2A54(const PokedexScreenManager *param0)
{
    const BannerSettings *bannerSettings = param0->operationalData;
    return bannerSettings->unk_28;
}

static BannerSettings *NewBannerSettings(enum HeapId heapID, PokedexApp *pokedexApp)
{
    BannerSettings *bannerSettings;
    PokedexScreenManager *screenMan;

    bannerSettings = Heap_AllocFromHeap(heapID, sizeof(BannerSettings));

    GF_ASSERT(bannerSettings);
    memset(bannerSettings, 0, sizeof(BannerSettings));

    bannerSettings->pageBinary = PokedexMain_GetPageBinary(pokedexApp);
    bannerSettings->pokedexSortData = PokedexMain_GetSortData(pokedexApp);
    bannerSettings->page = POKEDEX_PAGES_INFO;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_INFO);
    bannerSettings->infoSprite = screenMan->operationalData;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_FORMS);
    bannerSettings->formScreen = screenMan;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_FOREIGN);
    bannerSettings->foreignScreen = screenMan;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_AREA);
    bannerSettings->areaScreen = screenMan;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_CRY);
    bannerSettings->cryScreen = screenMan;

    screenMan = PokedexMain_GetBottomScreen(pokedexApp, POKEDEX_BOTTOM_SIZE);
    bannerSettings->sizeScreen = screenMan;

    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_EXIT);
    bannerSettings->scrollData = screenMan->operationalData;

    bannerSettings->unk_2C = 1;
    bannerSettings->unk_30 = 1;

    return bannerSettings;
}

static PokedexGraphicData **GetGraphicData1(enum HeapId heapID, PokedexApp *pokedexApp)
{
    PokedexGraphicData **graphicData = Heap_AllocFromHeap(heapID, sizeof(PokedexGraphicData *));

    GF_ASSERT(graphicData);
    memset(graphicData, 0, sizeof(PokedexGraphicData *));

    *graphicData = PokedexMain_GetGraphicData(pokedexApp);

    return graphicData;
}

static PokedexScreenState *NewBannerScreenState(enum HeapId heapID, PokedexApp *pokedexApp)
{
    PokedexScreenState *pokedexScreenState;
    int numScreenStates = NumBannerStates();

    pokedexScreenState = Heap_AllocFromHeap(heapID, sizeof(PokedexScreenState) * numScreenStates);

    GF_ASSERT(pokedexScreenState);
    memset(pokedexScreenState, 0, sizeof(PokedexScreenState) * numScreenStates);

    ov21_021D47F0(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_EXIT], pokedexApp, (0x1 << 1));
    ov21_021D4A94(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_INFO], pokedexApp, (0x1 << 2));
    ov21_021D48B8(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_AREA], pokedexApp, (0x1 << 3));
    ov21_021D491C(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_CRY], pokedexApp, (0x1 << 4));
    ov21_021D4980(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_SIZE], pokedexApp, (0x1 << 5));
    ov21_021D49E4(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_HEIGHT], pokedexApp, (0x1 << 6));
    ov21_021D4A3C(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_WEIGHT], pokedexApp, (0x1 << 7));
    ov21_021D4B50(heapID, &pokedexScreenState[POKEDEX_BANNER_STATES_FORMS], pokedexApp, (0x1 << 8));

    return pokedexScreenState;
}

static void FreeBannerSettings(BannerSettings *bannerSettings)
{
    GF_ASSERT(bannerSettings);
    Heap_FreeToHeap(bannerSettings);
}

static void FreeGraphicData1(PokedexGraphicData **graphicData)
{
    GF_ASSERT(graphicData);
    Heap_FreeToHeap(graphicData);
}

static void FreeScreenState(PokedexScreenState *pokedexScreenState)
{
    GF_ASSERT(pokedexScreenState);

    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_EXIT]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_INFO]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_AREA]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_CRY]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_SIZE]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_HEIGHT]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_WEIGHT]);
    ov21_021D4660(&pokedexScreenState[POKEDEX_BANNER_STATES_FORMS]);

    Heap_FreeToHeap(pokedexScreenState);
}

static int NumBannerStates(void)
{
    return MAX_POKEDEX_BANNER_STATES;
}

static int ov21_021E2C24(PokedexDataManager *param0, void *data)
{
    BannerSettings *bannerSettings = data;

    bannerSettings->infoSprite->cryPending = 1;

    return 1;
}

static int ov21_021E2C2C(PokedexDataManager *param0, void *data)
{
    BannerSettings *bannerSettings = data;

    if (param0->exit == TRUE) {
        return TRUE;
    }

    if (param0->unchanged == TRUE) {
        return FALSE;
    }

    if (param0->state == 0) {
        ov21_021E3320(bannerSettings);
        param0->state++;
    }

    return FALSE;
}

static int SetPageToInfo(PokedexDataManager *param0, void *data)
{
    BannerSettings *bannerSettings = data;

    bannerSettings->page = POKEDEX_PAGES_INFO;
    return 1;
}

static int EnterBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    const BannerSettings *bannerSettings = param2;
    PokedexGraphicData **pokedexGraphicData = param0;
    PageBannerData *pageBannerData = graphicsMan->pageGraphics;
    BOOL v4;

    switch (graphicsMan->state) {
    case 0:
        graphicsMan->pageGraphics = Heap_AllocFromHeap(graphicsMan->heapID, sizeof(PageBannerData));
        memset(graphicsMan->pageGraphics, 0, sizeof(PageBannerData));
        pageBannerData = graphicsMan->pageGraphics;
        pageBannerData->page = bannerSettings->page;
        graphicsMan->state++;
        break;
    case 1:
        DisplayBanner(pokedexGraphicData, pageBannerData, bannerSettings, graphicsMan->heapID);
        PageBannerXLU(pageBannerData);
        ov21_021D23F8(&(*pokedexGraphicData)->unk_168, 4, -16, 0, 0, 16, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD), (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD), 0);
        graphicsMan->state++;
        break;
    case 2:
        if (ov21_021D2424(&(*pokedexGraphicData)->unk_168)) {
            graphicsMan->state++;
        }
        break;
    case 3:
        PageBannerNormal(pageBannerData);
        return 1;
    default:
        GF_ASSERT(0);
        break;
    }

    return 0;
}

static int UpdateBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    const BannerSettings *bannerSettings = param2;
    PageBannerData *pageBannerData = graphicsMan->pageGraphics;

    if (pageBannerData->page != bannerSettings->page) {
        dummy5(pageBannerData, bannerSettings->page);
        UpdateBannerDisplay(pageBannerData, bannerSettings->page, pageBannerData->page);
        pageBannerData->page = bannerSettings->page;
    }

    return 0;
}

static int ExitBanner(void *param0, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    const BannerSettings *bannerSettings = param2;
    PokedexGraphicData **pokedexGraphicData = param0;
    PageBannerData *pageBannerData = graphicsMan->pageGraphics;

    switch (graphicsMan->state) {
    case 0:
        if (bannerSettings->unk_2C) {
            PageBannerXLU(pageBannerData);

            ov21_021D23F8(&(*pokedexGraphicData)->unk_168, 4, 0, -16, 16, 0, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD), (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD), 0);
        } else {
            ov21_021D23F8(&(*pokedexGraphicData)->unk_168, 4, 0, -16, 16, 0, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), 0, 0);
        }
        graphicsMan->state++;
        break;
    case 1:
        if (ov21_021D2424(&(*pokedexGraphicData)->unk_168)) {
            graphicsMan->state++;
        }
        break;
    case 2:
        FreeBanner(pokedexGraphicData, pageBannerData);
        graphicsMan->state++;
        break;
    case 3:
        Heap_FreeToHeap(graphicsMan->pageGraphics);
        graphicsMan->pageGraphics = NULL;
        graphicsMan->state++;
        break;
    case 4:
        return 1;
    default:
        GF_ASSERT(0);
        break;
    }

    return 0;
}

static void ExitEntry(BannerSettings *bannerSettings)
{
    ResetDisplay(bannerSettings);
    bannerSettings->unk_30 = 1;
}

static void ResetDisplay(BannerSettings *bannerSettings)
{
    *bannerSettings->pageBinary |= SCREEN_SCROLL;
    bannerSettings->unk_28 = 0;
    bannerSettings->infoSprite->unk_14 = 0;

    ov21_021E0CDC(bannerSettings->foreignScreen, 0);

    switch (bannerSettings->page) {
    case POKEDEX_PAGES_INFO:
        bannerSettings->infoSprite->x = 56;
        bannerSettings->infoSprite->y = 80;
        ov21_021E0CD4(bannerSettings->foreignScreen, 56, 80);
        bannerSettings->scrollData->unk_14 = 1;
        bannerSettings->unk_2C = 1;
        break;
    case POKEDEX_PAGES_AREA:
        ov21_021DCA4C(bannerSettings->areaScreen, 1);
        ov21_021DCA54(bannerSettings->areaScreen, 4);
        ov21_021DCA44(bannerSettings->areaScreen, 56, 80);
        bannerSettings->scrollData->unk_14 = 1;
        bannerSettings->unk_2C = 1;
        break;
    case POKEDEX_PAGES_CRY:
        ov21_021E4070(bannerSettings->cryScreen, 1);
        ov21_021E4078(bannerSettings->cryScreen, 4);
        ov21_021E4068(bannerSettings->cryScreen, 56, 80);
        bannerSettings->scrollData->unk_14 = 1;
        bannerSettings->unk_2C = 1;
        break;
    case POKEDEX_PAGES_SIZE:
        bannerSettings->scrollData->unk_14 = 0;
        bannerSettings->unk_2C = 0;
        break;
    case POKEDEX_PAGES_FORMS:
        bannerSettings->scrollData->unk_14 = 0;
        bannerSettings->unk_2C = 0;
        break;
    default:
        break;
    }
}

static BOOL ReturnFalse(enum PokedexPages page)
{
    return FALSE;
}

static BOOL SetCurrentPage(BannerSettings *bannerSettings, enum PokedexPages newPage, enum PokedexPages oldPage)
{
    int x, y;
    BOOL pageAccessible = TRUE;

    switch (newPage) {
    case POKEDEX_PAGES_INFO:
        if (ReturnFalse(oldPage)) {
            bannerSettings->infoSprite->unk_14 = 2;
        } else {
            bannerSettings->infoSprite->unk_14 = 1;
        }

        *bannerSettings->pageBinary |= SCREEN_INFO;
        bannerSettings->page = POKEDEX_PAGES_INFO;
        x = 48;
        y = 72;
        break;
    case POKEDEX_PAGES_AREA:
        if (ReturnFalse(oldPage)) {
            ov21_021DCA4C(bannerSettings->areaScreen, 1);
            ov21_021DCA54(bannerSettings->areaScreen, 1);
        } else {
            ov21_021DCA4C(bannerSettings->areaScreen, 0);
        }

        *bannerSettings->pageBinary |= SCREEN_AREA;
        bannerSettings->page = POKEDEX_PAGES_AREA;
        x = 40;
        y = 120;
        break;
    case POKEDEX_PAGES_CRY:
        if (ReturnFalse(oldPage)) {
            ov21_021E4070(bannerSettings->cryScreen, 1);
            ov21_021E4078(bannerSettings->cryScreen, 1);
        } else {
            ov21_021E4070(bannerSettings->cryScreen, 0);
        }

        *bannerSettings->pageBinary |= SCREEN_CRY;
        bannerSettings->page = POKEDEX_PAGES_CRY;
        x = 48;
        y = 64;
        break;
    case POKEDEX_PAGES_SIZE:
        if (PokedexSort_CurrentCaughtStatus(bannerSettings->pokedexSortData) == CS_CAUGHT) {
            *bannerSettings->pageBinary |= SCREEN_SIZE;
            bannerSettings->page = POKEDEX_PAGES_SIZE;
        } else {
            pageAccessible = FALSE;
        }
        break;
    case POKEDEX_PAGES_FORMS:
        if (PokedexSort_CanDetectForms(bannerSettings->pokedexSortData) == TRUE) {
            *bannerSettings->pageBinary |= SCREEN_FORMS;
            bannerSettings->page = POKEDEX_PAGES_FORMS;
        } else {
            pageAccessible = FALSE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    switch (oldPage) {
    case POKEDEX_PAGES_INFO:
        if (ReturnFalse(newPage)) {
            bannerSettings->infoSprite->unk_14 = 2;
            ov21_021E0CDC(bannerSettings->foreignScreen, 2);
        } else {
            bannerSettings->infoSprite->unk_14 = 1;
            ov21_021E0CDC(bannerSettings->foreignScreen, 1);
        }

        bannerSettings->infoSprite->x = x;
        bannerSettings->infoSprite->y = y;
        ov21_021E0CD4(bannerSettings->foreignScreen, x, y);
        break;
    case POKEDEX_PAGES_AREA:
        if (ReturnFalse(newPage)) {
            ov21_021DCA4C(bannerSettings->areaScreen, 1);
            ov21_021DCA54(bannerSettings->areaScreen, 1);
        } else {
            ov21_021DCA4C(bannerSettings->areaScreen, 0);
        }

        ov21_021DCA44(bannerSettings->areaScreen, x, y);
        break;
    case POKEDEX_PAGES_CRY:
        if (ReturnFalse(newPage)) {
            ov21_021E4070(bannerSettings->cryScreen, 1);
            ov21_021E4078(bannerSettings->cryScreen, 1);
        } else {
            ov21_021E4070(bannerSettings->cryScreen, 0);
        }

        ov21_021E4068(bannerSettings->cryScreen, x, y);
        break;
    case POKEDEX_PAGES_SIZE:
        break;
    case POKEDEX_PAGES_FORMS:
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return pageAccessible;
}

static void DisplayBanner(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, const BannerSettings *bannerSettings, int heapID)
{
    InitTopScreenGraphics(pokedexGraphicData, bannerSettings, heapID);
    SetPageBannerSprites(pokedexGraphicData, pageBannerData, heapID);
    dummy3(pokedexGraphicData, pageBannerData, heapID);
    DisplayPageName(pokedexGraphicData, pageBannerData, heapID);
    UpdateBannerDisplay(pageBannerData, POKEDEX_PAGES_INFO, POKEDEX_PAGES_AREA);
}

static void FreeBanner(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData)
{
    dummy4(pageBannerData);
    FreeBannerGraphics(pokedexGraphicData, pageBannerData);
    FreeBannerTextMan(pageBannerData);

    Bg_ClearTilemap((*pokedexGraphicData)->bgConfig, 2);
}

static void InitTopScreenGraphics(PokedexGraphicData **pokedexGraphicData, const BannerSettings *bannerSettings, int heapID)
{
    NNSG2dScreenData *ppScrData;

    Pokedex_LoadGraphicNarcPaletteData(*pokedexGraphicData, 6, 0, 0, 0, heapID);

    if (PokedexSort_IsNationalDex(bannerSettings->pokedexSortData) == TRUE) {
        Pokedex_LoadGraphicNarcPaletteData(*pokedexGraphicData, 24, 0, 0, 32, heapID);
    }

    Pokedex_LoadGraphicNarcCharacterData(*pokedexGraphicData, 33, (*pokedexGraphicData)->bgConfig, 2, 0, 0, TRUE, heapID);

    void *pokedexGraphic = Pokedex_GetGraphicNarcScreenData(*pokedexGraphicData, 57, 1, &ppScrData, heapID);

    Bg_LoadToTilemapRect((*pokedexGraphicData)->bgConfig, 2, ppScrData->rawData, 0, 0, ppScrData->screenWidth / 8, ppScrData->screenHeight / 8);
    Heap_FreeToHeap(pokedexGraphic);
    Bg_ScheduleTilemapTransfer((*pokedexGraphicData)->bgConfig, 2);
}

static void SetPageBannerSprites(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int heapID)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;
    NARC *pokedexGraphicsNARC = Pokedex_PokedexGraphicsNARC(*pokedexGraphicData);

    pageBannerData->spriteResource[SPRITE_RESOURCE_CHAR] = SpriteResourceCollection_AddTilesFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CHAR], pokedexGraphicsNARC, 87, 1, 87 + 2000, NNS_G2D_VRAM_TYPE_2DMAIN, heapID);

    SpriteTransfer_RequestCharAtEnd(pageBannerData->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteResource_ReleaseData(pageBannerData->spriteResource[SPRITE_RESOURCE_CHAR]);

    pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT] = SpriteResourceCollection_AddPaletteFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_PLTT], pokedexGraphicsNARC, 12, 0, 12 + 2000, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapID);

    SpriteTransfer_RequestPlttFreeSpace(pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT]);
    SpriteResource_ReleaseData(pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT]);

    pageBannerData->spriteResource[SPRITE_RESOURCE_CELL] = SpriteResourceCollection_AddFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CELL], pokedexGraphicsNARC, 85, 1, 85 + 2000, SPRITE_RESOURCE_CELL, heapID);
    pageBannerData->spriteResource[SPRITE_RESOURCE_ANIM] = SpriteResourceCollection_AddFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_ANIM], pokedexGraphicsNARC, 86, 1, 86 + 2000, SPRITE_RESOURCE_ANIM, heapID);
}

static void FreeBannerGraphics(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;

    SpriteTransfer_ResetCharTransfer(pageBannerData->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteTransfer_ResetPlttTransfer(pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT]);

    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CHAR], pageBannerData->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_PLTT], pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CELL], pageBannerData->spriteResource[SPRITE_RESOURCE_CELL]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_ANIM], pageBannerData->spriteResource[SPRITE_RESOURCE_ANIM]);
}

static void dummy3(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int heapID)
{
    return;
}

static void dummy4(PageBannerData *pageBannerData)
{
    return;
}

static void DisplayPageName(PokedexGraphicData **pokedexGraphicData, PageBannerData *pageBannerData, int heapID)
{
    Window *window;
    PokedexDisplayBox displayBox;
    PokedexGraphicData *graphicData = *pokedexGraphicData;
    int plttOffset;

    displayBox.unk_00 = graphicData->unk_14C;
    displayBox.paletteProxy = SpriteTransfer_GetPaletteProxy(pageBannerData->spriteResource[SPRITE_RESOURCE_PLTT], NULL);
    displayBox.x = 16;
    displayBox.y = 0;
    displayBox.unk_18 = 0;
    displayBox.unk_1C = 0;
    displayBox.sprite = NULL;
    displayBox.vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
    displayBox.heapID = heapID;

    plttOffset = PlttTransfer_GetPlttOffset(displayBox.paletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN);

    for (int page = 0; page < MAX_POKEDEX_PAGES; page++) {
        window = Pokedex_NewWindow(graphicData->unk_14C, 8, 2);

        Pokedex_DisplayMessage(graphicData->unk_14C, window, TEXT_BANK_POKEDEX, pl_msg_pokedex_info + page, 0, 0);

        displayBox.window = window;
        pageBannerData->bannerTextMan[page] = ov21_021D4CA0(&displayBox);

        sub_02012A60(pageBannerData->bannerTextMan[page]->fontOAM, plttOffset);
        sub_020129D0(pageBannerData->bannerTextMan[page]->fontOAM, FALSE);

        Pokedex_FreeWindow(window);
    }
}

static void FreeBannerTextMan(PageBannerData *pageBannerData)
{
    for (int page = 0; page < MAX_POKEDEX_PAGES; page++) {
        ov21_021D4D1C(pageBannerData->bannerTextMan[page]);
    }
}

static void dummy5(PageBannerData *pageBannerData, int param1)
{
    return;
}

static void ov21_021E3320(BannerSettings *bannerSettings)
{
    bannerSettings->unk_28 = 1;
    bannerSettings->infoSprite->unk_14 = 1;
}

void PokedexEntry_PanelTransitions(PokedexScreenManager *pokedexScreenMan, PokedexApp *param1, enum HeapId heapID)
{
    PageTransitionData *pageTransitionData;
    PokedexGraphicData **pokedexGraphicData;
    PokedexScreenState *pokedexScreenState;

    pageTransitionData = NewPageTransitionData(heapID, param1);
    pokedexGraphicData = GetGraphicData2(heapID, param1);
    pokedexScreenState = NullScreenState(heapID, param1);

    pokedexScreenMan->operationalData = pageTransitionData;
    pokedexScreenMan->graphicData = pokedexGraphicData;
    pokedexScreenMan->pokedexScreenState = pokedexScreenState;
    pokedexScreenMan->numScreenStates = NumPanelStates();

    pokedexScreenMan->dataTransition[POKEDEX_TRANSITION_IN] = ResetBottomData;
    pokedexScreenMan->dataTransition[POKEDEX_TRANSITION_UPDATE] = UpdateEntryPage;
    pokedexScreenMan->dataTransition[POKEDEX_TRANSITION_OUT] = FreeBottomData;
    pokedexScreenMan->displayTransition[POKEDEX_TRANSITION_IN] = EnterPokedexEntry;
    pokedexScreenMan->displayTransition[POKEDEX_TRANSITION_UPDATE] = UpdateBottomDisplay;
    pokedexScreenMan->displayTransition[POKEDEX_TRANSITION_OUT] = ExitPokedexEntry;
}

void PokedexEntry_FreeTransitionData(PokedexScreenManager *pokedexScreenMan)
{
    FreePageTransitionData(pokedexScreenMan->operationalData);
    FreeGraphicData2(pokedexScreenMan->graphicData);
    dummy6(pokedexScreenMan->pokedexScreenState);
}

BOOL ov21_021E33A4(const PokedexScreenManager *param0)
{
    const PageTransitionData *pageTransitionData = param0->operationalData;
    return pageTransitionData->bottomScreenFade;
}

BOOL ov21_021E33AC(const PokedexScreenManager *param0)
{
    const PageTransitionData *pageTransitionData = param0->operationalData;
    return pageTransitionData->bottomScreenInteractible;
}

void ov21_021E33B4(PokedexScreenManager *param0, BOOL cursorOnTarget)
{
    PageTransitionData *pageTransitionData = param0->operationalData;
    pageTransitionData->cursorOnTarget = cursorOnTarget;
}

BOOL ov21_021E33BC(const PokedexScreenManager *param0)
{
    const PageTransitionData *pageTransitionData = param0->operationalData;
    return pageTransitionData->cursorOnTarget;
}

static PageTransitionData *NewPageTransitionData(enum HeapId heapID, PokedexApp *pokedexApp)
{
    PageTransitionData *pageTransitionData;
    PokedexScreenManager *screenMan;

    pageTransitionData = Heap_AllocFromHeap(heapID, sizeof(PageTransitionData));

    GF_ASSERT(pageTransitionData);
    memset(pageTransitionData, 0, sizeof(PageTransitionData));

    pageTransitionData->unk_00 = ov21_021D13C8(pokedexApp);
    screenMan = PokedexMain_GetTopScreen(pokedexApp, POKEDEX_TOP_CLEAR);
    pageTransitionData->bannerSettings = screenMan->operationalData;
    pageTransitionData->sortData = PokedexMain_GetSortData(pokedexApp);

    return pageTransitionData;
}

static PokedexGraphicData **GetGraphicData2(enum HeapId heapID, PokedexApp *pokedexApp)
{
    PokedexGraphicData **graphicData = Heap_AllocFromHeap(heapID, sizeof(PokedexGraphicData *));

    GF_ASSERT(graphicData);
    memset(graphicData, 0, sizeof(PokedexGraphicData *));

    *graphicData = PokedexMain_GetGraphicData(pokedexApp);

    return graphicData;
}

static PokedexScreenState *NullScreenState(enum HeapId heapID, PokedexApp *param1)
{
    return NULL;
}

static void FreePageTransitionData(PageTransitionData *pageTransitionData)
{
    GF_ASSERT(pageTransitionData);
    Heap_FreeToHeap(pageTransitionData);
}

static void FreeGraphicData2(PokedexGraphicData **graphicData)
{
    GF_ASSERT(graphicData);
    Heap_FreeToHeap(graphicData);
}

static void dummy6(PokedexScreenState *pokedexScreenState)
{
    return;
}

static int NumPanelStates(void)
{
    return 0;
}

static int ResetBottomData(PokedexDataManager *param0, void *param1)
{
    PageTransitionData *pageTransitionData = param1;
    PokedexBottomData *pokedexBottomData = param0->pageData;

    pokedexBottomData = Heap_AllocFromHeap(param0->heapID, sizeof(PokedexBottomData));
    memset(pokedexBottomData, 0, sizeof(PokedexBottomData));

    InitHitTable(pokedexBottomData, pageTransitionData, param0->heapID);
    InitCursorData(pokedexBottomData, pageTransitionData, param0->heapID);
    CursorToActivePage(pokedexBottomData, pageTransitionData);

    for (int button = 0; button < MAX_POKEDEX_BUTTONS; button++) {
        pageTransitionData->buttonStatus[button] = POKEDEX_BUTTON_INIT;
    }

    param0->pageData = pokedexBottomData;

    return TRUE;
}

static int UpdateEntryPage(PokedexDataManager *param0, void *param1)
{
    PageTransitionData *pageTransitionData = param1;
    PokedexBottomData *pokedexBottomData = param0->pageData;

    if (param0->exit == TRUE) {
        pageTransitionData->bottomScreenFade = FALSE;
        return TRUE;
    }

    if (param0->unchanged == TRUE) {
        return FALSE;
    }

    if (param0->state == 0) {
        pageTransitionData->bottomScreenFade = TRUE;
        pageTransitionData->bannerSettings->unk_30 = 0;
        pageTransitionData->bottomScreenInteractible = TRUE;
        param0->state++;
    } else {
        pageTransitionData->bottomScreenInteractible = TRUE;

        CheckForExit(pageTransitionData);
        CheckForSpeciesChange(pokedexBottomData, pageTransitionData);
        DeactivateAllButtons(pageTransitionData);

        if ((ov21_021D3998(pageTransitionData->sortData) == 2) && (pageTransitionData->cursorOnTarget == FALSE)) {
            PressButton(pokedexBottomData, pageTransitionData);
        }

        SetActiveButton(pokedexBottomData, pageTransitionData);
    }

    return FALSE;
}

static int FreeBottomData(PokedexDataManager *param0, void *param1)
{
    PokedexBottomData *pokedexBottomData = param0->pageData;

    FreeHitTable(pokedexBottomData);
    freePanel(pokedexBottomData);

    Heap_FreeToHeap(param0->pageData);
    param0->pageData = NULL;

    return 1;
}

static int EnterPokedexEntry(void *data, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    const PageTransitionData *pageTransitionData = param2;
    const PokedexBottomData *pokedexBottomData = param3->pageData;
    PokedexGraphicData **pokedexGraphicData = data;
    PageButtonGraphics *pageButtonGraphics = graphicsMan->pageGraphics;

    switch (graphicsMan->state) {
    case 0:
        graphicsMan->pageGraphics = Heap_AllocFromHeap(graphicsMan->heapID, sizeof(PageButtonGraphics));
        memset(graphicsMan->pageGraphics, 0, sizeof(PageButtonGraphics));
        graphicsMan->state++;
        break;
    case 1:
        DisplayBottomScreen(pageButtonGraphics, pokedexGraphicData, pageTransitionData, graphicsMan->heapID);
        graphicsMan->state++;
        break;
    case 2:
        BrightnessController_StartTransition(4, 0, -16, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), BRIGHTNESS_SUB_SCREEN);
        CursorToCurrentButton(pokedexGraphicData, pokedexBottomData, pageTransitionData);
        graphicsMan->state++;
        break;
    case 3:
        if (BrightnessController_IsTransitionComplete(BRIGHTNESS_SUB_SCREEN)) {
            graphicsMan->state++;
        }
        break;
    case 4:
        return 1;
    default:
        break;
    }

    return 0;
}

static int UpdateBottomDisplay(void *data, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    const PageTransitionData *pageTransitionData = param2;
    const PokedexBottomData *pokedexBottomData = param3->pageData;
    PokedexGraphicData **pokedexGraphicData = data;
    PageButtonGraphics *pageButtonGraphics = graphicsMan->pageGraphics;

    for (int button = 0; button < MAX_POKEDEX_BUTTONS; button++) {
        PokedexMain_ButtonAnimation(pageButtonGraphics->sprite[button], pageTransitionData->buttonStatus[button]);
    }

    UpdateCursor(pokedexGraphicData, pokedexBottomData, pageTransitionData);

    return FALSE;
}

static int ExitPokedexEntry(void *data, PokedexGraphicsManager *graphicsMan, const void *param2, const PokedexDataManager *param3)
{
    PokedexGraphicData **pokedexGraphicData = data;
    PageButtonGraphics *pageButtonGraphics = graphicsMan->pageGraphics;

    switch (graphicsMan->state) {
    case 0:
        BrightnessController_StartTransition(4, -16, 0, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), BRIGHTNESS_SUB_SCREEN);
        graphicsMan->state++;
        break;
    case 1:
        if (BrightnessController_IsTransitionComplete(BRIGHTNESS_SUB_SCREEN)) {
            graphicsMan->state++;
        }
        break;
    case 2:
        FreePageButtons(pageButtonGraphics, pokedexGraphicData);
        PokedexCursor_SetCursorVisbility(&(*pokedexGraphicData)->cursor, FALSE);
        graphicsMan->state++;
        break;
    case 3:
        Heap_FreeToHeap(graphicsMan->pageGraphics);
        graphicsMan->pageGraphics = NULL;
        graphicsMan->state++;
        break;
    case 4:
        return 1;
    default:
        break;
    }

    return 0;
}

static void InitHitTable(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData, enum HeapId heapID)
{
    pokedexBottomData->touchScreenHitTable = Heap_AllocFromHeap(heapID, sizeof(TouchScreenHitTable) * MAX_POKEDEX_BUTTONS);

    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_INFO], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_INFO_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_INFO_X + (BUTTON_HITBOX_WIDTH / 2));
    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_EXIT], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_EXIT_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_EXIT_X + (BUTTON_HITBOX_WIDTH / 2));
    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_AREA], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_AREA_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_AREA_X + (BUTTON_HITBOX_WIDTH / 2));
    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_CRY], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_CRY_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_CRY_X + (BUTTON_HITBOX_WIDTH / 2));
    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_SIZE], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_SIZE_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_SIZE_X + (BUTTON_HITBOX_WIDTH / 2));
    PokedexMain_SetHitTableBoundaries(&pokedexBottomData->touchScreenHitTable[POKEDEX_BUTTONS_FORMS], BUTTON_Y - (BUTTON_HITBOX_HEIGHT / 2), BUTTON_Y + (BUTTON_HITBOX_HEIGHT / 2), BUTTON_FORMS_X - (BUTTON_HITBOX_WIDTH / 2), BUTTON_FORMS_X + (BUTTON_HITBOX_WIDTH / 2));

    pokedexBottomData->unk_08.unk_00 = pageTransitionData;
    pokedexBottomData->unk_08.unk_04 = pokedexBottomData;
    pokedexBottomData->unk_00 = sub_02023FCC(pokedexBottomData->touchScreenHitTable, MAX_POKEDEX_BUTTONS, ButtonEffects, &pokedexBottomData->unk_08, heapID);
}

static void DeactivateAllButtons(PageTransitionData *pageTransitionData)
{
    for (int button = 0; button < MAX_POKEDEX_BUTTONS; button++) {
        pageTransitionData->buttonStatus[button] = POKEDEX_BUTTON_DEACTIVE;
    }
}

static void SetActiveButton(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData)
{
    if (pageTransitionData->exiting == FALSE) {
        sub_0202404C(pokedexBottomData->unk_00);
    }

    if (pageTransitionData->exiting) {
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_EXIT] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_EXIT] = POKEDEX_BUTTON_ACTIVE;
        }
    }

    switch (pageTransitionData->bannerSettings->page) {
    case POKEDEX_PAGES_INFO:
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_INFO] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_INFO] = POKEDEX_BUTTON_ACTIVE;
        }
        break;
    case POKEDEX_PAGES_AREA:
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_AREA] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_AREA] = POKEDEX_BUTTON_ACTIVE;
        }
        break;
    case POKEDEX_PAGES_CRY:
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_CRY] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_CRY] = POKEDEX_BUTTON_ACTIVE;
        }
        break;
    case POKEDEX_PAGES_SIZE:
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_SIZE] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_SIZE] = POKEDEX_BUTTON_ACTIVE;
        }
        break;
    case POKEDEX_PAGES_FORMS:
        if (pageTransitionData->buttonStatus[POKEDEX_BUTTONS_FORMS] != POKEDEX_BUTTON_PRESSED) {
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_FORMS] = POKEDEX_BUTTON_ACTIVE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static void FreeHitTable(PokedexBottomData *pokedexBottomData)
{
    sub_02024034(pokedexBottomData->unk_00);
    Heap_FreeToHeap(pokedexBottomData->touchScreenHitTable);
    pokedexBottomData->touchScreenHitTable = NULL;
}

static void ButtonEffects(u32 button, u32 buttonStatus, void *data)
{
    UnkStruct_ov21_021E37CC *v0 = data;
    PageTransitionData *pageTransitionData = v0->unk_00;
    PokedexBottomData *v2 = v0->unk_04;
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;

    pageTransitionData->buttonStatus[button] = buttonStatus;

    switch (buttonStatus) {
    case POKEDEX_BUTTON_DEFAULT:
        v2->unk_10 = 0;
        break;
    case POKEDEX_BUTTON_PRESSED:
        PokedexPanel_Move(v2->pokedexPanelData, PANEL_MOVEMENT_COL, button);

        switch (button) {
        case POKEDEX_BUTTONS_EXIT:
            ExitEntry(bannerSettings);
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            break;
        case POKEDEX_BUTTONS_INFO:
            if (bannerSettings->page != POKEDEX_PAGES_INFO) {
                GoToPage(pageTransitionData, POKEDEX_PAGES_INFO, bannerSettings->page);
                CursorToActivePage(v2, pageTransitionData);
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_AREA:
            if (bannerSettings->page != POKEDEX_PAGES_AREA) {
                GoToPage(pageTransitionData, POKEDEX_PAGES_AREA, bannerSettings->page);
                CursorToActivePage(v2, pageTransitionData);
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_CRY:
            if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                GoToPage(pageTransitionData, POKEDEX_PAGES_CRY, bannerSettings->page);
                CursorToActivePage(v2, pageTransitionData);
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_SIZE:
            if (bannerSettings->page != POKEDEX_PAGES_SIZE) {
                if (GoToPage(pageTransitionData, POKEDEX_PAGES_SIZE, bannerSettings->page)) {
                    CursorToActivePage(v2, pageTransitionData);
                    Sound_PlayEffect(SEQ_SE_DP_DENSI06);
                } else {
                    if (v2->unk_10 == 0) {
                        if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
                        }

                        v2->unk_10 = 1;
                    }
                }
            }
            break;
        case POKEDEX_BUTTONS_FORMS:
            if (bannerSettings->page != POKEDEX_PAGES_FORMS) {
                if (GoToPage(pageTransitionData, POKEDEX_PAGES_FORMS, bannerSettings->page)) {
                    CursorToActivePage(v2, pageTransitionData);
                    Sound_PlayEffect(SEQ_SE_DP_DENSI06);
                } else {
                    if (v2->unk_10 == 0) {
                        if (PokedexSort_CanDetectForms(bannerSettings->pokedexSortData) == TRUE) {
                            if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                                Sound_PlayEffect(SEQ_SE_DP_DECIDE);
                            }
                        }

                        v2->unk_10 = 0;
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

static void DisplayBottomScreen(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, const PageTransitionData *pageTransitionData, int heapID)
{
    InitBottomScreenGraphics(pokedexGraphicData, pageTransitionData, heapID);
    SetPageButtonSprites(pageButtonGraphics, pokedexGraphicData, heapID);
    DisplayPageButtons(pageButtonGraphics, pokedexGraphicData, heapID);
    Dummy1(pageButtonGraphics, pokedexGraphicData, heapID);
    DisplayFormButton(pageButtonGraphics, pageTransitionData);
}

static void FreePageButtons(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData)
{
    Dummy2(pageButtonGraphics);
    RemovePageButtons(pageButtonGraphics);
    FreePageButtonGraphics(pageButtonGraphics, pokedexGraphicData);
    Bg_ClearTilemap((*pokedexGraphicData)->bgConfig, 5);
}

static void InitBottomScreenGraphics(PokedexGraphicData **pokedexGraphicData, const PageTransitionData *pageTransitionData, int heapID)
{
    NNSG2dScreenData *ppScrData;

    if (pageTransitionData->bannerSettings->unk_30) {
        Pokedex_LoadGraphicNarcPaletteData(*pokedexGraphicData, 6, 4, 0, 0, heapID);

        if (PokedexSort_IsNationalDex(pageTransitionData->sortData) == TRUE) {
            Pokedex_LoadGraphicNarcPaletteData(*pokedexGraphicData, 24, 4, 0, 32, heapID);
        }
    }

    Pokedex_LoadGraphicNarcCharacterData(*pokedexGraphicData, 34, (*pokedexGraphicData)->bgConfig, 5, 0, 0, 1, heapID);

    void *pokedexGraphic = Pokedex_GetGraphicNarcScreenData(*pokedexGraphicData, 59, TRUE, &ppScrData, heapID);

    Bg_LoadToTilemapRect((*pokedexGraphicData)->bgConfig, 5, ppScrData->rawData, 0, 0, ppScrData->screenWidth / 8, ppScrData->screenHeight / 8);
    Heap_FreeToHeap(pokedexGraphic);
    Bg_ScheduleTilemapTransfer((*pokedexGraphicData)->bgConfig, 5);
}

static void SetPageButtonSprites(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;
    NARC *pokedexGraphicsNARC = Pokedex_PokedexGraphicsNARC(*pokedexGraphicData);

    pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CHAR] = SpriteResourceCollection_AddTilesFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CHAR], pokedexGraphicsNARC, 96, 1, 96 + 2100, NNS_G2D_VRAM_TYPE_2DSUB, heapID);

    SpriteTransfer_RequestCharAtEnd(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteResource_ReleaseData(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CHAR]);

    pageButtonGraphics->spriteResource[SPRITE_RESOURCE_PLTT] = SpriteResourceCollection_AddPaletteFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_PLTT], pokedexGraphicsNARC, 11, 0, 11 + 2100, NNS_G2D_VRAM_TYPE_2DSUB, 6, heapID);

    SpriteTransfer_RequestPlttFreeSpace(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_PLTT]);
    SpriteResource_ReleaseData(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_PLTT]);

    pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CELL] = SpriteResourceCollection_AddFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CELL], pokedexGraphicsNARC, 94, 1, 94 + 2100, SPRITE_RESOURCE_CELL, heapID);
    pageButtonGraphics->spriteResource[SPRITE_RESOURCE_ANIM] = SpriteResourceCollection_AddFrom(graphicData->spriteResourceCollection[SPRITE_RESOURCE_ANIM], pokedexGraphicsNARC, 95, 1, 95 + 2100, SPRITE_RESOURCE_ANIM, heapID);
}

static void FreePageButtonGraphics(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;

    SpriteTransfer_ResetCharTransfer(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteTransfer_ResetPlttTransfer(pageButtonGraphics->spriteResource[SPRITE_RESOURCE_PLTT]);

    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CHAR], pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CHAR]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_PLTT], pageButtonGraphics->spriteResource[SPRITE_RESOURCE_PLTT]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_CELL], pageButtonGraphics->spriteResource[SPRITE_RESOURCE_CELL]);
    SpriteResourceCollection_Remove(graphicData->spriteResourceCollection[SPRITE_RESOURCE_ANIM], pageButtonGraphics->spriteResource[SPRITE_RESOURCE_ANIM]);
}

static void DisplayPageButtons(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID)
{
    SpriteResourcesHeader spriteResourcesHeader;
    SpriteListTemplate spriteListTemplate;
    PokedexGraphicData *graphicData = *pokedexGraphicData;

    SpriteResourcesHeader_Init(&spriteResourcesHeader, 96 + 2100, 11 + 2100, 94 + 2100, 95 + 2100, 0xffffffff, 0xffffffff, 0, 0, graphicData->spriteResourceCollection[SPRITE_RESOURCE_CHAR], graphicData->spriteResourceCollection[SPRITE_RESOURCE_PLTT], graphicData->spriteResourceCollection[SPRITE_RESOURCE_CELL], graphicData->spriteResourceCollection[SPRITE_RESOURCE_ANIM], NULL, NULL);

    spriteListTemplate.list = graphicData->spriteList;
    spriteListTemplate.resourceData = &spriteResourcesHeader;
    spriteListTemplate.priority = 32;
    spriteListTemplate.vramType = NNS_G2D_VRAM_TYPE_2DSUB;
    spriteListTemplate.heapID = heapID;
    spriteListTemplate.position.y = (BUTTON_Y * FX32_ONE) + (192 << FX32_SHIFT);

    for (int button = 0; button < MAX_POKEDEX_BUTTONS; button++) {
        switch (button) {
        case POKEDEX_BUTTONS_EXIT:
            spriteListTemplate.position.x = BUTTON_EXIT_X * FX32_ONE;
            break;
        case POKEDEX_BUTTONS_INFO:
            spriteListTemplate.position.x = BUTTON_INFO_X * FX32_ONE;
            break;
        case POKEDEX_BUTTONS_AREA:
            spriteListTemplate.position.x = BUTTON_AREA_X * FX32_ONE;
            break;
        case POKEDEX_BUTTONS_CRY:
            spriteListTemplate.position.x = BUTTON_CRY_X * FX32_ONE;
            break;
        case POKEDEX_BUTTONS_SIZE:
            spriteListTemplate.position.x = BUTTON_SIZE_X * FX32_ONE;
            break;
        case POKEDEX_BUTTONS_FORMS:
            spriteListTemplate.position.x = BUTTON_FORMS_X * FX32_ONE;
            break;
        }

        pageButtonGraphics->sprite[button] = SpriteList_Add(&spriteListTemplate);
        Sprite_SetAnim(pageButtonGraphics->sprite[button], button);
    }
}

static void RemovePageButtons(PageButtonGraphics *pageButtonGraphics)
{
    for (int button = 0; button < MAX_POKEDEX_BUTTONS; button++) {
        Sprite_Delete(pageButtonGraphics->sprite[button]);
    }
}

static void Dummy1(PageButtonGraphics *pageButtonGraphics, PokedexGraphicData **pokedexGraphicData, int heapID)
{
    return;
}

static void Dummy2(PageButtonGraphics *pageButtonGraphics)
{
    return;
}

static void PageBannerXLU(PageBannerData *pageBannerData)
{
    for (int page = 0; page < MAX_POKEDEX_PAGES; page++) {
        sub_02012AF0(pageBannerData->bannerTextMan[page]->fontOAM, GX_OAM_MODE_XLU);
    }
}

static void PageBannerNormal(PageBannerData *pageBannerData)
{
    for (int page = 0; page < MAX_POKEDEX_PAGES; page++) {
        sub_02012AF0(pageBannerData->bannerTextMan[page]->fontOAM, GX_OAM_MODE_NORMAL);
    }
}

static BOOL GoToPage(PageTransitionData *pageTransitionData, enum PokedexPages newPage, enum PokedexPages oldPage)
{
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;
    BOOL pageAccessible = SetCurrentPage(bannerSettings, newPage, oldPage);

    if (pageAccessible == TRUE) {
        pageTransitionData->bottomScreenInteractible = FALSE;
    }

    return pageAccessible;
}

static void CheckForExit(PageTransitionData *pageTransitionData)
{
    CheckForB(pageTransitionData);
}

static void CheckForB(PageTransitionData *pageTransitionData)
{
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;

    setExitingFalse(pageTransitionData);

    if (gSystem.pressedKeys & PAD_BUTTON_B) {
        ExitEntry(bannerSettings);
        pageTransitionData->exiting = TRUE;
        Sound_PlayEffect(SEQ_SE_DP_DECIDE);
        return;
    }
}

static void setExitingFalse(PageTransitionData *pageTransitionData)
{
    pageTransitionData->exiting = FALSE;
}

static void InitCursorData(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData, enum HeapId heapID)
{
    enum CursorMovementType cursorMovementType;

    pokedexBottomData->pokedexCursorData = Heap_AllocFromHeap(heapID, sizeof(PokedexCursorData) * MAX_POKEDEX_BUTTONS);

    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_INFO], BUTTON_INFO_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, CURSOR_MOVEMENT_SELECT, CURSOR_MOVEMENT_SELECT, POKEDEX_BUTTONS_INFO);
    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_AREA], BUTTON_AREA_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, CURSOR_MOVEMENT_SELECT, CURSOR_MOVEMENT_SELECT, POKEDEX_BUTTONS_AREA);
    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_CRY], BUTTON_CRY_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, CURSOR_MOVEMENT_SELECT, CURSOR_MOVEMENT_SELECT, POKEDEX_BUTTONS_CRY);
    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_SIZE], BUTTON_SIZE_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, CURSOR_MOVEMENT_SELECT, CURSOR_MOVEMENT_SELECT, POKEDEX_BUTTONS_SIZE);

    if (PokedexSort_CanDetectForms(pageTransitionData->sortData) == TRUE) {
        cursorMovementType = CURSOR_MOVEMENT_SELECT;
    } else {
        cursorMovementType = CURSOR_MOVEMENT_SKIP;
    }

    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_FORMS], BUTTON_FORMS_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, cursorMovementType, cursorMovementType, POKEDEX_BUTTONS_FORMS);
    PokedexPanel_InitPokedexCursorData(&pokedexBottomData->pokedexCursorData[POKEDEX_BUTTONS_EXIT], BUTTON_EXIT_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, CURSOR_MOVEMENT_SELECT, CURSOR_MOVEMENT_SELECT, POKEDEX_BUTTONS_EXIT);

    pokedexBottomData->pokedexPanelData = PokedexPanel_Alloc(heapID);
    PokedexPanel_InitPokedexPanelData(pokedexBottomData->pokedexPanelData, pokedexBottomData->pokedexCursorData, MAX_POKEDEX_BUTTONS, 1);
}

static void PressButton(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData)
{
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;

    if (pageTransitionData->exiting) {
        return;
    }

    if (gSystem.pressedKeys & PAD_KEY_RIGHT) {
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_MULTICOL, 1);
    }

    if (gSystem.pressedKeys & PAD_KEY_LEFT) {
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_MULTICOL, -1);
    }

    if (gSystem.pressedKeys & PAD_BUTTON_A) {
        switch (PokedexPanel_GetCurrentButton(pokedexBottomData->pokedexPanelData)) {
        case POKEDEX_BUTTONS_INFO:
            if (bannerSettings->page != POKEDEX_PAGES_INFO) {
                GoToPage(pageTransitionData, POKEDEX_BUTTONS_INFO, bannerSettings->page);
                pageTransitionData->buttonStatus[POKEDEX_BUTTONS_INFO] = POKEDEX_BUTTON_PRESSED;
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_AREA:
            if (bannerSettings->page != POKEDEX_PAGES_AREA) {
                GoToPage(pageTransitionData, POKEDEX_BUTTONS_AREA, bannerSettings->page);
                pageTransitionData->buttonStatus[POKEDEX_BUTTONS_AREA] = POKEDEX_BUTTON_PRESSED;
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_CRY:
            if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                GoToPage(pageTransitionData, POKEDEX_BUTTONS_CRY, bannerSettings->page);
                pageTransitionData->buttonStatus[POKEDEX_BUTTONS_CRY] = POKEDEX_BUTTON_PRESSED;
                Sound_PlayEffect(SEQ_SE_DP_DENSI06);
            }
            break;
        case POKEDEX_BUTTONS_SIZE:
            if (bannerSettings->page != POKEDEX_PAGES_SIZE) {
                if (GoToPage(pageTransitionData, POKEDEX_BUTTONS_SIZE, bannerSettings->page)) {
                    pageTransitionData->buttonStatus[POKEDEX_BUTTONS_SIZE] = POKEDEX_BUTTON_PRESSED;
                    Sound_PlayEffect(SEQ_SE_DP_DENSI06);
                } else {
                    if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                        Sound_PlayEffect(SEQ_SE_DP_DECIDE);
                    }
                }
            }
            break;
        case POKEDEX_BUTTONS_FORMS:
            if (bannerSettings->page != POKEDEX_PAGES_FORMS) {
                if (GoToPage(pageTransitionData, POKEDEX_BUTTONS_FORMS, bannerSettings->page)) {
                    pageTransitionData->buttonStatus[POKEDEX_BUTTONS_FORMS] = POKEDEX_BUTTON_PRESSED;
                    Sound_PlayEffect(SEQ_SE_DP_DENSI06);
                } else {
                    if (bannerSettings->page != POKEDEX_PAGES_CRY) {
                        Sound_PlayEffect(SEQ_SE_DP_DECIDE);
                    }
                }
            }
            break;
        case POKEDEX_BUTTONS_EXIT:
            ExitEntry(bannerSettings);
            pageTransitionData->exiting = TRUE;
            pageTransitionData->buttonStatus[POKEDEX_BUTTONS_EXIT] = 2;
            Sound_PlayEffect(SEQ_SE_DP_DECIDE);
            break;
        }
    }
}

static void freePanel(PokedexBottomData *pokedexBottomData)
{
    Heap_FreeToHeap(pokedexBottomData->pokedexCursorData);
    PokedexPanel_Free(pokedexBottomData->pokedexPanelData);
    pokedexBottomData->pokedexPanelData = NULL;
    pokedexBottomData->pokedexCursorData = NULL;
}

static void CursorToActivePage(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData)
{
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;

    pageTransitionData->cursorOnTarget = FALSE;

    switch (bannerSettings->page) {
    case POKEDEX_PAGES_INFO:
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_JUMP, POKEDEX_BUTTONS_INFO);
        break;
    case POKEDEX_PAGES_AREA:
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_JUMP, POKEDEX_BUTTONS_AREA);
        break;
    case POKEDEX_PAGES_CRY:
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_JUMP, POKEDEX_BUTTONS_CRY);
        break;
    case POKEDEX_PAGES_SIZE:
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_JUMP, POKEDEX_BUTTONS_SIZE);
        break;
    case POKEDEX_PAGES_FORMS:
        PokedexPanel_Move(pokedexBottomData->pokedexPanelData, PANEL_MOVEMENT_JUMP, POKEDEX_BUTTONS_FORMS);
        break;
    default:
        break;
    }
}

static void CheckForSpeciesChange(PokedexBottomData *pokedexBottomData, PageTransitionData *pageTransitionData)
{
    BannerSettings *bannerSettings = pageTransitionData->bannerSettings;

    if (bannerSettings->page == POKEDEX_PAGES_INFO) {
        return;
    }

    if (ov21_021D3998(pageTransitionData->sortData) != 2) {
        return;
    }

    if (pageTransitionData->cursorOnTarget == FALSE) {
        if (gSystem.pressedKeys & PAD_KEY_DOWN) {
            pageTransitionData->cursorOnTarget = TRUE;
        }
    } else {
        if (gSystem.pressedKeys & PAD_KEY_UP) {
            pageTransitionData->cursorOnTarget = FALSE;
        }
    }
}

static void CursorToCurrentButton(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData, const PageTransitionData *pageTransitionData)
{
    PokedexCursor_GoToCurrentButton(pokedexBottomData->pokedexPanelData, *pokedexGraphicData);
    UpdateCursor(pokedexGraphicData, pokedexBottomData, pageTransitionData);
}

static void UpdateCursor(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData, const PageTransitionData *pageTransitionData)
{
    if (ov21_021D3998(pageTransitionData->sortData) != 0) {
        BOOL visibilityChanged = UpdateCursorVisibility(pokedexGraphicData, TRUE);

        if (pageTransitionData->cursorOnTarget == FALSE) {
            if (visibilityChanged) {
                PokedexCursor_GoToCurrentButton(pokedexBottomData->pokedexPanelData, *pokedexGraphicData);
            }

            CursorTransformToCurrent(pokedexGraphicData, pokedexBottomData);
        }
    } else {
        UpdateCursorVisibility(pokedexGraphicData, FALSE);
    }
}

static void CursorTransformToCurrent(PokedexGraphicData **pokedexGraphicData, const PokedexBottomData *pokedexBottomData)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;
    PokedexCursor_CurrentButtonTransformation(pokedexBottomData->pokedexPanelData, graphicData);
}

static BOOL UpdateCursorVisibility(PokedexGraphicData **pokedexGraphicData, BOOL draw)
{
    PokedexGraphicData *graphicData = *pokedexGraphicData;

    if (PokedexCursor_GetCursorVisbility(&graphicData->cursor) != draw) {
        PokedexCursor_SetCursorVisbility(&graphicData->cursor, draw);
        return TRUE;
    }

    return FALSE;
}

static void UpdateBannerDisplay(PageBannerData *pageBannerData, enum PokedexPages newPage, enum PokedexPages oldPage)
{
    sub_020129D0(pageBannerData->bannerTextMan[oldPage]->fontOAM, FALSE);
    sub_020129D0(pageBannerData->bannerTextMan[newPage]->fontOAM, TRUE);
}

static void DisplayFormButton(PageButtonGraphics *pageButtonGraphics, const PageTransitionData *pageTransitionData)
{
    if (PokedexSort_CanDetectForms(pageTransitionData->sortData) == FALSE) {
        Sprite_SetDrawFlag(pageButtonGraphics->sprite[POKEDEX_BUTTONS_FORMS], FALSE);
    }
}
