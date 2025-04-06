#include "overlay021/ov21_021D4C0C.h"

#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02012744_decl.h"
#include "struct_defs/struct_020127E8.h"

#include "overlay021/pokedex_display_box.h"
#include "overlay021/pokedex_text_manager.h"
#include "overlay021/struct_ov21_021D1FA4.h"

#include "bg_window.h"
#include "char_transfer.h"
#include "font.h"
#include "heap.h"
#include "message.h"
#include "sprite.h"
#include "strbuf.h"
#include "text.h"
#include "unk_02012744.h"

typedef struct UnkStruct_ov21_021D4C0C_t {
    UnkStruct_02012744 *unk_00;
    SpriteList *spriteList;
    BgConfig *bgConfig;
    PokedexTextManager *unk_0C;
    int unk_10;
    enum HeapId heapID;
} UnkStruct_ov21_021D4C0C;

static BOOL ov21_021D4EA4(PokedexTextManager *param0);
static PokedexTextManager *ov21_021D4EB4(UnkStruct_ov21_021D4C0C *param0);

UnkStruct_ov21_021D4C0C *ov21_021D4C0C(const UnkStruct_ov21_021D1FA4 *param0)
{
    UnkStruct_ov21_021D4C0C *v0;

    v0 = Heap_AllocFromHeap(param0->unk_0C, sizeof(UnkStruct_ov21_021D4C0C));

    GF_ASSERT(v0);
    memset(v0, 0, sizeof(UnkStruct_ov21_021D4C0C));

    v0->unk_00 = sub_02012744(param0->unk_08, param0->unk_0C);
    v0->unk_0C = Heap_AllocFromHeap(param0->unk_0C, sizeof(PokedexTextManager) * param0->unk_08);

    GF_ASSERT(v0->unk_0C);
    memset(v0->unk_0C, 0, sizeof(PokedexTextManager) * param0->unk_08);

    v0->unk_10 = param0->unk_08;
    v0->spriteList = param0->unk_00;
    v0->bgConfig = param0->unk_04;
    v0->heapID = param0->unk_0C;

    return v0;
}

void ov21_021D4C6C(UnkStruct_ov21_021D4C0C *param0)
{
    GF_ASSERT(param0);

    ov21_021D4D3C(param0);
    Heap_FreeToHeap(param0->unk_0C);
    sub_020127BC(param0->unk_00);

    memset(param0, 0, sizeof(UnkStruct_ov21_021D4C0C));

    Heap_FreeToHeap(param0);
}

PokedexTextManager *ov21_021D4CA0(const PokedexDisplayBox *param0)
{
    int size = sub_02012898(param0->window, param0->vramType, param0->heapID);
    return ov21_021D4CB8(param0, size);
}

PokedexTextManager *ov21_021D4CB8(const PokedexDisplayBox *param0, int size)
{
    UnkStruct_020127E8 v0;
    PokedexTextManager *v1;

    v1 = ov21_021D4EB4(param0->unk_00);

    GF_ASSERT(v1);
    CharTransfer_AllocRange(size, TRUE, param0->vramType, &v1->charTransferAllocation);

    v0.unk_00 = param0->unk_00->unk_00;
    v0.unk_04 = param0->window;
    v0.unk_08 = param0->unk_00->spriteList;
    v0.unk_0C = param0->paletteProxy;
    v0.unk_10 = param0->sprite;
    v0.unk_14 = v1->charTransferAllocation.offset;
    v0.unk_18 = param0->x;
    v0.unk_1C = param0->y;
    v0.unk_20 = param0->unk_18;
    v0.unk_24 = param0->unk_1C;
    v0.unk_28 = param0->vramType;

    v0.unk_2C = param0->heapID;

    v1->fontOAM = sub_020127E8(&v0);

    return v1;
}

void ov21_021D4D1C(PokedexTextManager *param0)
{
    sub_02012870(param0->fontOAM);
    CharTransfer_ClearRange(&param0->charTransferAllocation);

    memset(param0, 0, sizeof(PokedexTextManager));
}

void ov21_021D4D3C(UnkStruct_ov21_021D4C0C *param0)
{
    int v0;

    for (v0 = 0; v0 < param0->unk_10; v0++) {
        if (ov21_021D4EA4(&param0->unk_0C[v0])) {
            ov21_021D4D1C(&param0->unk_0C[v0]);
        }
    }
}

Window *Pokedex_NewWindow(UnkStruct_ov21_021D4C0C *param0, int width, int height)
{
    Window *window = Window_New(param0->heapID, 1);

    Window_Init(window);
    Window_AddToTopLeftCorner(param0->bgConfig, window, width, height, 0, 0);

    return window;
}

void Pokedex_FreeWindow(Window *window)
{
    Windows_Delete(window, 1);
}

u32 Pokedex_DisplayMessage(UnkStruct_ov21_021D4C0C *param0, Window *window, u32 bankID, u32 entryID, int xOffset, int yOffset)
{
    MessageLoader *messageLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, bankID, param0->heapID);
    GF_ASSERT(messageLoader);

    Strbuf *strbuf = MessageLoader_GetNewStrbuf(messageLoader, entryID);
    Text_AddPrinterWithParamsAndColor(window, FONT_SUBSCREEN, strbuf, xOffset, yOffset, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(3, 2, 1), NULL);
    u32 strWidth = Font_CalcStrbufWidth(FONT_SUBSCREEN, strbuf, 0);

    Strbuf_Free(strbuf);
    MessageLoader_Free(messageLoader);

    return strWidth;
}

void Pokedex_DisplayMessageBottomCentered(UnkStruct_ov21_021D4C0C *param0, Window *window, u32 bankID, u32 entryID)
{
    MessageLoader *messageLoader = MessageLoader_Init(MESSAGE_LOADER_BANK_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, bankID, param0->heapID);
    GF_ASSERT(messageLoader);

    Strbuf *strbuf = MessageLoader_GetNewStrbuf(messageLoader, entryID);
    int strbufWidth = Font_CalcStrbufWidth(FONT_SUBSCREEN, strbuf, 0);
    int xOffset = Window_GetWidth(window) * 8;
    xOffset -= strbufWidth;
    xOffset /= 2;

    Text_AddPrinterWithParamsAndColor(window, FONT_SUBSCREEN, strbuf, xOffset, 0, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(3, 2, 1), NULL);
    Strbuf_Free(strbuf);
    MessageLoader_Free(messageLoader);
}

void Pokedex_DisplayText(UnkStruct_ov21_021D4C0C *param0, Window *window, Strbuf *strbuf, int xOffset, int yOffset)
{
    Text_AddPrinterWithParamsAndColor(window, FONT_SUBSCREEN, strbuf, xOffset, yOffset, TEXT_SPEED_NO_TRANSFER, TEXT_COLOR(3, 2, 1), NULL);
}

static BOOL ov21_021D4EA4(PokedexTextManager *param0)
{
    if (param0->fontOAM) {
        return 1;
    }

    return 0;
}

static PokedexTextManager *ov21_021D4EB4(UnkStruct_ov21_021D4C0C *param0)
{
    int v0;

    for (v0 = 0; v0 < param0->unk_10; v0++) {
        if (ov21_021D4EA4(&param0->unk_0C[v0]) == 0) {
            return &param0->unk_0C[v0];
        }
    }

    return NULL;
}
