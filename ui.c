#include "ui.h"
#include "UITileset.h"
#include "libfchucker.h"
#include "windowstack.h"
#include "memcard.h"
#include "inventory.h"

char MCSlotAccesStr[256] = "[ Memory Card Slot 2]";
char MCAccesStr[256] = {0};
char HelpText[256] = {0};
int HelpTextID = -1;
int HelpTextHeight = 1;
float tileZed = 0.0f;
UITile_t GetTile(int tileID)
{
    UITile_t result = *(UITile_t *)(UITileset + ((4 * tileID)));
    return result;
}

void PrintTileInfo(const char *name, UITile_t tile)
{
    printf("TILE %s: Width: %d, Height: %d, X: %d, Y: %d\n", name, tile.width, tile.height, tile.x, tile.y);
}

void DrawTileID(int x, int y, int TILEID)
{
    UITile_t t = GetTile(TILEID);
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, t.x, t.y, x + t.width, y + t.height, 0.0f, t.x + t.width, t.y + t.height, TexColor);
}

void DrawTileSizedID(int x, int y, int width, int height, int TILEID)
{
    UITile_t t = GetTile(TILEID);
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, t.x, t.y, x + width, y + height, 0.0f, t.x + t.width, t.y + t.height, TexColor);
}

void DrawTile(int x, int y, UITile_t tile)
{
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, tile.x, tile.y, x + tile.width, y + tile.height, 0.0f, tile.x + tile.width, tile.y + tile.height, TexColor);
}

void DrawTileSized(int x, int y, int width, int height, UITile_t tile)
{
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, tile.x, tile.y, x + width, y + height, 0.0f, tile.x + tile.width, tile.y + tile.height, TexColor);

}

void DrawTileF(float x, float y, UITile_t tile)
{
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, tile.x, tile.y, x + tile.width, y + tile.height, 0.0f, tile.x + tile.width, tile.y + tile.height, TexColor);
}

void DrawTileSizedF(float x, float y, float width, float height, UITile_t tile)
{
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsUITex, x, y, tileZed, tile.x, tile.y, x + width, y + height, 0.0f, tile.x + tile.width, tile.y + tile.height, TexColor);

}


void DrawWindowBG(int x, int y, int width, int height)
{
    UITile_t tl = GetTile(TILE_Window_Top_Left);
    UITile_t tm = GetTile(TILE_Window_Top_Mid);
    UITile_t tr = GetTile(TILE_Window_Top_Right);

    UITile_t ml = GetTile(TILE_Window_Mid_Left);
    UITile_t mm = GetTile(TILE_Window_Mid_Mid);
    UITile_t mr = GetTile(TILE_Window_Mid_Right);

    UITile_t bl = GetTile(TILE_Window_Bottom_Left);
    UITile_t bm = GetTile(TILE_Window_Bottom_Mid);
    UITile_t br = GetTile(TILE_Window_Bottom_Right);

    int mw = width - (tl.width + tr.width);
    int mh = height - (tl.height + bl.height);
    DrawTile(x, y, tl);
    DrawTileSized(x + tl.width, y, mw, tm.height, tm);
    DrawTile(x + tl.width + mw, y, tr);

    DrawTileSized(x, y + tl.height, ml.width, mh, ml);
    DrawTileSized(x + ml.width, y + tm.height, mw, mh, mm);
    DrawTileSized(x + ml.width + mw, y + mr.height, mr.width, mh, mr);

    DrawTile(x, y + tl.height + mh, bl );
    DrawTileSized(x + bl.width, y + tm.height + mh, mw, bm.height, bm);
    DrawTile(x + bl.width + mw, y + mr.height + mh, br);
}

void DrawAsciiBar(int x, int y, char * text)
{
    UITile_t barLeft = GetTile(TILE_AsciiBar_Left);
    UITile_t barRight = GetTile(TILE_AsciiBar_Right);

    int currentX = x;
    DrawTile(x, y, barLeft);
    currentX += barLeft.width;

    unsigned long i = 0;
    for(i = 0; i < strlen(text); i++)
    {
        unsigned char c = (unsigned char)text[i];
        if(c > 0x1f)
        {
            if(c == 0x20)
            {
                UITile_t t = GetTile(TILE_AsciiBar_Space);
                DrawTile(currentX, y, t);
                currentX += t.width;
                continue;
            }
            else if(c >= 0x30 && c < 0x3a)
            {
                UITile_t t = GetTile(TILE_AsciiBar_0 + (c - 0x30));
                DrawTile(currentX, y, t);
                currentX += t.width;
                continue;
            }
            else if(c >= 0x41 && c < 0x91)
            {
                UITile_t t = GetTile(TILE_AsciiBar_A + (c - 0x41));
                DrawTile(currentX, y, t);
                currentX += t.width;
                continue;
            }
        }
    }

    DrawTile(currentX, y, barRight);
}

void DrawHDiv(int x, int y, int width)
{
    UITile_t left = GetTile(TILE_Window_Div_Horizontal_Left);
    UITile_t mid = GetTile(TILE_Window_Div_Horizontal_Mid);
    UITile_t right = GetTile(TILE_Window_Div_Horizontal_Right);

    int midWidth = width - (left.width + right.width);
    DrawTile(x, y, left);
    DrawTileSized(x + left.width, y, midWidth, mid.height, mid);
    DrawTile(x + left.width + midWidth, y, right);
}

void DrawSelectionBox(int x, int y, int width, int height)
{
    UITile_t tl = GetTile(TILE_Square_Selection_Rect_Top_Left);
    UITile_t tm = GetTile(TILE_Square_Selection_Rect_Top_Mid);
    UITile_t tr = GetTile(TILE_Square_Selection_Rect_Top_Right);

    UITile_t ml = GetTile(TILE_Square_Selection_Rect_Mid_Left);
    UITile_t mm = GetTile(TILE_Square_Selection_Rect_Mid_Mid);
    UITile_t mr = GetTile(TILE_Square_Selection_Rect_Mid_Right);

    UITile_t bl = GetTile(TILE_Square_Selection_Rect_Bottom_Left);
    UITile_t bm = GetTile(TILE_Square_Selection_Rect_Bottom_Mid);
    UITile_t br = GetTile(TILE_Square_Selection_Rect_Bottom_Right);

    int dy = y;
    int mh = height - (tl.height + bl.height);
    int mw = width - (tl.width + tr.width);
    DrawTile(x, y, tl);
    DrawTileSized(x + tl.width, y, mw, tm.height, tm);
    DrawTile(x + tl.width + mw, y, tr);
    dy += tl.height;

    if(height > (tl.height + br.height))
    {
        DrawTileSized(x, dy, ml.width, mh, ml);
        DrawTileSized(x + ml.width, dy, mw, mh, mm);
        DrawTileSized(x + ml.width + mw, dy, mr.width, mh, mr);
        dy += mh;
    }

    DrawTile(x, dy, bl);
    DrawTileSized(x + bl.width, dy, mw, bm.height, bm);
    DrawTile(x + bl.width + mw, dy, br);
}

void DrawHScrollBar(int x, int y, int step, int visibleAreaHeight, int fullAreaHeight)
{
    //Draw Scrollbar Bg
    UITile_t bgt = GetTile(TILE_Scrollbar_Top);
    UITile_t bgm = GetTile(TILE_Scrollbar_Middle);
    UITile_t bgb = GetTile(TILE_Scrollbar_Bottom);
    UITile_t ht = GetTile(TILE_ScrollbarHandle_Top);
    UITile_t hm = GetTile(TILE_ScrollbarHandle_Middle);
    UITile_t hb = GetTile(TILE_ScrollbarHandle_Bottom);

    int bgmh = visibleAreaHeight - (bgt.height + bgb.height);
    int dy = y;
    DrawTile(x, dy, bgt);
    dy += bgt.height;
    DrawTileSized(x, dy, bgm.width, bgmh, bgm);
    dy += bgmh;
    DrawTile(x, dy, bgb);

    //Now draw handle
    float viewableRatio = (float)visibleAreaHeight / (float)fullAreaHeight;
    float thumbHeight = (float)visibleAreaHeight * viewableRatio;

    float trackSpace = (float)fullAreaHeight - (float)visibleAreaHeight;
    float thumbSpace = (float)visibleAreaHeight - thumbHeight;
    float scrollJump = trackSpace / thumbSpace;
    float thumbHeightMid = thumbHeight - (ht.height + hb.height);

    float scrollOffset = (float)(step * 20) / scrollJump;
    float dyy =y + scrollOffset;
    DrawTileF(x, dyy, ht);
    dyy += ht.height;
    DrawTileSizedF(x, dyy, hm.width, thumbHeightMid, hm);
    dyy += thumbHeightMid;
    DrawTileF(x, dyy, hb);

}

#define INVENTORY_DRAW_ICON 0
#define INVENTORY_DRAW_TEXT 17
#define INVENTORY_DRAW_QTY 200
#define INVENTORY_DRAW_TRANSFER 242

void DrawInventoryItem(int x, int y, InventoryItem_t * item, int transferAmount)
{
    int iconID = TILE_Icon_Exclaim;
    switch (item->Category) {
    case ITEM_TYPE_TWINBLADE:
        iconID = TILE_Icon_TwinBlade;
        break;
    case ITEM_TYPE_BLADEMASTER:
        iconID = TILE_Icon_Blade;
        break;
    case ITEM_TYPE_HEAVYBLADE:
        iconID = TILE_Icon_HeavyBlade;
        break;
    case ITEM_TYPE_HEAVEAXE:
        iconID = TILE_Icon_Axe;
        break;
    case ITEM_TYPE_LONGARM:
        iconID = TILE_Icon_Spear;
        break;
    case ITEM_TYPE_WAVEMASTER:
        iconID = TILE_Icon_Staff;
        break;
    case ITEM_TYPE_HELMET:
        iconID = TILE_Icon_Helmet;
        break;
    case ITEM_TYPE_ARMOR:
        iconID = TILE_Icon_Armor;
        break;
    case ITEM_TYPE_HANDARMOR:
        iconID = TILE_Icon_HandArmor;
        break;
    case ITEM_TYPE_LEGARMOR:
        iconID = TILE_Icon_LegArmor;
        break;
    case ITEM_TYPE_ITEM:
        iconID = TILE_Icon_Item;
        break;
    case ITEM_TYPE_SCROLLS:
        iconID = TILE_Icon_Scroll;
        break;
    case ITEM_TYPE_SPECIAL_USE:
    case ITEM_TYPE_TREASURE:
    default:
        iconID = TILE_Icon_Exclaim;
    }


    DrawTileID(x + INVENTORY_DRAW_ICON, y, iconID);
    unsigned char * itemName = GetItemName(item->Category, item->ItemID);
    DrawString(x + INVENTORY_DRAW_TEXT, y, itemName);
    unsigned char itemQtyBuffer[16] = {0};

    sprintf((char *)itemQtyBuffer, "%2d", item->Count);
    DrawString(x + INVENTORY_DRAW_QTY, y, itemQtyBuffer);

    if(transferAmount > 0)
    {
        sprintf((char *)itemQtyBuffer, "%2d", transferAmount);
        DrawString(x + INVENTORY_DRAW_TRANSFER, y, itemQtyBuffer);
        DrawTileID(x + INVENTORY_DRAW_TRANSFER - 16, y, TILE_BlueArrow_Right);
    }
}

int CurrentInventoryWindow = 0;
#define TAB_WIDTH 136
void DrawTabStrip(int x, int y, int tabCount, int currentTab, unsigned char * currentTabText)
{
    int dx = x ;
    UITile_t TabLeft = GetTile(TILE_Tab_Left);
    UITile_t TabMid = GetTile(TILE_Tab_Mid);
    UITile_t TabRight = GetTile(TILE_Tab_Right);
    UITile_t TabLeftO = GetTile(TILE_Tab_Left_Overlapped);
    UITile_t TabRightO = GetTile(TILE_Tab_Right_Overlapped);
    UITile_t TabLeftOB = GetTile(TILE_Tab_Window_Overlay_Left);
    UITile_t TabMidOB = GetTile(TILE_Tab_Window_Overlay_Mid);
    UITile_t TabRightOB = GetTile(TILE_Tab_Window_Overlay_Right);
    DrawTile(x, y, TabLeft);
    dx += TabLeft.width;
    int i = 0;
    int curTabX = x;

    for(i = 0; i < tabCount; i++)
    {
        //Draw Mid
        if(i < currentTab)
        {
            DrawTile(dx, y, TabLeftO);
            dx += TabLeftO.width;
        }

        if(currentTab == i)
        {


            DrawTileSized(dx, y, TAB_WIDTH, TabMid.height, TabMid);
            curTabX = dx;
            DrawTile(dx - TabLeftOB.width, y + TabMid.height  - 4, TabLeftOB);
            DrawTileSized(dx, y + TabMid.height - 4, TAB_WIDTH, TabMidOB.height, TabMidOB);
            dx += TAB_WIDTH;
            DrawTile(dx, y + TabMid.height  - 4, TabRightOB);

        }

        if(i >= currentTab)
        {
            //Draw Right
            if(i < (tabCount - 1))
            {
                DrawTile(dx, y, TabRightO);
                dx += TabRightO.width;
            }
            else
            {
                DrawTile(dx, y, TabRight);
                dx += TabRight.width;
            }

        }
    }
    DrawString(curTabX + 2, y + 6, currentTabText);

    tileZed = 0.0f;
}

void GoInventoryView()
{
    if(SourceMCSlot == 0)
    {
        PushWindow(&DrawMC0AccessDialog_Window);
    }
    else
    {
        PushWindow(&DrawMC1AccessDialog_Window);
    }
    Draw();

    ReadCharacter(SourceMCSlot, SourceCharacterSlot, CHARACTER_SRC);
    EnumerateInventory(CHARACTER_SRC);

    PopWindow();
    Draw();
    if(DestMCSlot == 0)
    {
        PushWindow(&DrawMC0AccessDialog_Window);
    }
    else
    {
        PushWindow(&DrawMC1AccessDialog_Window);
    }
    Draw();
    ReadCharacter(DestMCSlot, DestCharacterSlot, CHARACTER_DEST);
    EnumerateInventory(CHARACTER_DEST);
    PopWindow();
    Draw();

    InitInventoryWindow(&InventoryWindows[0], CHARACTER_SRC);
    InitInventoryWindow(&InventoryWindows[1], CHARACTER_DEST);

    PushWindow(&DrawInventory_Window);
}

void GoBackToCharacterSelect()
{
    PopWindow();
    FreeInventory();
    SetInventoriesClean();
    PushWindow(&DrawCharSelectScreen_Window);
    RefreshCharacters(gsGlobal);

}


void SaveAndGoBackToCharacterSelect()
{
    if(SourceMCSlot == 0)
    {
        PushWindow(&DrawMC0AccessDialog_Window);
    }
    else
    {
        PushWindow(&DrawMC1AccessDialog_Window);
    }
    Draw();

    WriteCharacter(SourceMCSlot, SourceCharacterSlot, CHARACTER_SRC);

    Draw();
    PopWindow();
    if(DestMCSlot == 0)
    {
        PushWindow(&DrawMC0AccessDialog_Window);
    }
    else
    {
        PushWindow(&DrawMC1AccessDialog_Window);
    }
    Draw();

    WriteCharacter(DestMCSlot, DestCharacterSlot, CHARACTER_DEST);
    Draw();
    PopWindow();

    PopWindow();
    FreeInventory();
    SetInventoriesClean();
    PushWindow(&DrawCharSelectScreen_Window);
    RefreshCharacters();
}

ItemTransferResult_t currentTransferResult;
void DoItemTransfer()
{
    InventoryWindow_t * currentWindow = &InventoryWindows[CurrentInventoryWindow];
    currentTransferResult = TransferItem(currentWindow->CurrentItemRow, CurrentInventoryWindow, currentWindow->CurrentTransferAmount);

    PushWindow(&DrawTransferResult_Window);
}

void CloseTransferResultWindow()
{
    InventoryWindows[CurrentInventoryWindow].CurrentTransferAmount = 0;
    EnumerateInventory(0);
    EnumerateInventory(1);
    SetInventoryWindowCounts(&InventoryWindows[0]);
    SetInventoryWindowCounts(&InventoryWindows[1]);

    InventoryWindow_t * currentWindow = &InventoryWindows[CurrentInventoryWindow];
    int currentPage = currentWindow->CurrentPage;
    int * currentPageIndex = &currentWindow->Page0Index + currentPage;
    int * currentPageCount = &currentWindow->Page0Count + currentPage;
    int * currentScrollStep = &currentWindow->Page0ScrollStep + currentPage;


    if(currentTransferResult.totalLeft == 0)
    {
        if(*currentPageIndex > *currentPageCount - 1)
        {
            *currentPageIndex = (*currentPageCount - 1);
        }

        if(*currentScrollStep > 0) *currentScrollStep -= 1;

        if(*currentPageCount == 0)
        {
            *currentPageIndex = 0;
            *currentScrollStep = 0;
        }
    }

    PopWindow();
}


void DrawInventoryTransferResult()
{
    int sHeight = gsGlobal->Height;
    int sWidth = gsGlobal->Width;
    int wWidth = 512;
    int wHeight = (currentTransferResult.remaining > 0) ? 128: 96;
    int wX = (sWidth / 2) - (wWidth / 2);
    int wY = (sHeight / 2) - (wHeight / 2);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawAsciiBar(wX + INFO_BAR_OFFSET_X, wY + INFO_BAR_OFFSET_Y, "INFO");


    int rowSrc = (SourceMCSlot * 3) + SourceCharacterSlot;
    int rowDest = (DestMCSlot * 3) + DestCharacterSlot;
    if(CurrentInventoryWindow)
    {
        rowSrc = (DestMCSlot * 3) + DestCharacterSlot;
        rowDest = (SourceMCSlot * 3) + SourceCharacterSlot;
    }

    int dy = wY + 12;
    DrawBlitTex(rowDest, wX + 16, dy);
    char texBuffer[64];
    unsigned char * itemName = GetItemName(currentTransferResult.itemCategory, currentTransferResult.itemID);
    sprintf(texBuffer," received %s x %d", itemName, currentTransferResult.inventoryCount + currentTransferResult.storageCount);
    DrawString(wX + BlitLineWidths[rowDest] + 16, dy, (unsigned char *)texBuffer);
    dy += 20;


    sprintf(texBuffer, "Inventory: %d", currentTransferResult.inventoryCount);
    DrawString(wX + 16, dy, (unsigned char *)texBuffer);
    dy += 20;
    sprintf(texBuffer, "Storage: %d", currentTransferResult.storageCount);
    DrawString(wX + 16, dy , (unsigned char *)texBuffer);
    dy += 20;

    if(currentTransferResult.remaining > 0)
    {
        sprintf(texBuffer, "%d were not transfered due to inventory limitations.", currentTransferResult.remaining);
        DrawString(wX + 16, dy, (unsigned char *)texBuffer);
        dy += 20;

        DrawString(wX + 16, dy, (unsigned char*)GetTransferErrorMessage(currentTransferResult.resultFlags));
    }
}

InventoryWindow_t InventoryWindows[2];
void SetInventoryWindowCounts(InventoryWindow_t * window)
{
    window->Page0Count = 0;
    window->Page1Count = 0;
    window->Page2Count = 0;
    window->Page3Count = 0;
    window->Page4Count = 0;
    window->Page5Count = 0;
    window->Page6Count = 0;
    window->Page7Count = 0;
    //Iterate over inventory to get our counts...
    int i = 0;
    for(i = 0; i < (40); i++)
    {
        InventoryItem_t * t = (window->Inventory + i);
        if(t->Category < ITEM_TYPE_ITEM)
        {
            window->Page3Count++;
        }
        else if(t->Category == ITEM_TYPE_ITEM || t->Category == ITEM_TYPE_SPECIAL_USE)
        {
            window->Page0Count++;
        }
        else if(t->Category == ITEM_TYPE_SCROLLS)
        {
            window->Page1Count++;
        }
        else if(t->Category == ITEM_TYPE_TREASURE)
        {
            window->Page2Count++;
        }
    }

    for(i = 40; i < 139; i++)
    {
        InventoryItem_t * t = (window->Inventory + i);
        if(t->Category < ITEM_TYPE_ITEM)
        {
            window->Page7Count++;
        }
        else if(t->Category == ITEM_TYPE_ITEM || t->Category == ITEM_TYPE_SPECIAL_USE)
        {
            window->Page4Count++;
        }
        else if(t->Category == ITEM_TYPE_SCROLLS)
        {
            window->Page5Count++;
        }
        else if(t->Category == ITEM_TYPE_TREASURE)
        {
            window->Page6Count++;
        }
    }
}

void InitInventoryWindow(InventoryWindow_t * window, int dest)
{
    window->CurrentPage = 0;
    window->Page0Index = 0;
    window->Page1Index = 0;
    window->Page2Index = 0;
    window->Page3Index = 0;
    window->Page4Index = 0;
    window->Page5Index = 0;
    window->Page6Index = 0;
    window->Page7Index = 0;

    window->Page0Count = 0;
    window->Page1Count = 0;
    window->Page2Count = 0;
    window->Page3Count = 0;
    window->Page4Count = 0;
    window->Page5Count = 0;
    window->Page6Count = 0;
    window->Page7Count = 0;

    window->Page0ScrollStep = 0;
    window->Page1ScrollStep = 0;
    window->Page2ScrollStep = 0;
    window->Page3ScrollStep = 0;
    window->Page4ScrollStep = 0;
    window->Page5ScrollStep = 0;
    window->Page6ScrollStep = 0;
    window->Page7ScrollStep = 0;

    window->Inventory = (InventoryItem_t *)SourceInventory;
    window->CurrentItemRow = 0;
    window->CurrentTransferAmount = 0;
   window->StorageCount = 99 - SourceStorageCount;
    window->InventoryCount = 40 - SourceInventoryCount;
    if(dest)
    {
        window->Inventory = (InventoryItem_t *)DestInventory;
        window->StorageCount = 99 - DestStorageCount;
        window->InventoryCount = 40 - DestInventoryCount;
    }

    SetInventoryWindowCounts(window);
}

//TODO: make this stuff better?

unsigned char * InventoryTabNames[8] = {
    (unsigned char *)"Items",
    (unsigned char *)"Scrolls",
    (unsigned char *)"Treasure",
    (unsigned char *)"Equipment",
    (unsigned char *)"Items (Storage)",
    (unsigned char *)"Scrolls (Storage)",
    (unsigned char *)"Treasure (Storage)",
    (unsigned char *)"Equipment (Storage)"
};
void DrawInventoryWindow(int dest, int x, int y, int width, int height)
{
    InventoryWindow_t * currentWindow = &InventoryWindows[dest];

    DrawWindowBG(x, y, width, height);
    DrawTabStrip(x + 16, y - 16, 8, currentWindow->CurrentPage, InventoryTabNames[currentWindow->CurrentPage]);
    currentWindow->VisibleItemCount = ((height - 32) / 20);

    int dy = y + 16;
    int startIndex = 0;
    int endIndex = 40;
    int pageID = currentWindow->CurrentPage;
    int currentSelectionIndex = *(int*)(&currentWindow->Page0Index + pageID);
    int currentPageItemCount = *(int*)(&currentWindow->Page0Count + pageID);
    if(currentWindow->CurrentPage > 3)
    {
        startIndex = 40;
        endIndex = 139;
        pageID -= 4;
    }

    int i = 0;
    int currentScrollStep = *((int *)&currentWindow->Page0ScrollStep + currentWindow->CurrentPage);

    int drawCount = -1;
    for(i = startIndex; i < endIndex; i++)
    {
        InventoryItem_t * t = &currentWindow->Inventory[i];
        if(t->Category == 0xff) continue;
        int selectedItem = (drawCount == currentSelectionIndex - 1);
        if(pageID == 0)
        {
            if(t->Category == ITEM_TYPE_ITEM || t->Category == ITEM_TYPE_SPECIAL_USE)
            {
                drawCount++;
                if(drawCount < currentScrollStep) continue;
                if(drawCount >= (currentScrollStep + currentWindow->VisibleItemCount)) continue;
                DrawInventoryItem(x + 16, dy, t, selectedItem ? currentWindow->CurrentTransferAmount: 0);
                if(selectedItem) currentWindow->CurrentItemRow = i;
                dy += INVENTORY_LINE_HEIGHT;

            }
        }
        if(pageID == 1)
        {
            if(t->Category == ITEM_TYPE_SCROLLS)
            {
                drawCount++;
                if(drawCount < currentScrollStep) continue;
                if(drawCount >= (currentScrollStep + currentWindow->VisibleItemCount)) continue;
                DrawInventoryItem(x + 16, dy, t, (selectedItem  && CurrentInventoryWindow == dest)  ? currentWindow->CurrentTransferAmount: 0);
                if(selectedItem) currentWindow->CurrentItemRow = i;
                dy += INVENTORY_LINE_HEIGHT;
            }
        }
        if(pageID == 2)
        {
            if(t->Category == ITEM_TYPE_TREASURE)
            {
                drawCount++;
                if(drawCount < currentScrollStep) continue;
                if(drawCount >= (currentScrollStep + currentWindow->VisibleItemCount)) continue;
                DrawInventoryItem(x + 16, dy, t, (selectedItem  && CurrentInventoryWindow == dest) ? currentWindow->CurrentTransferAmount: 0);
                if(selectedItem) currentWindow->CurrentItemRow = i;
                dy += INVENTORY_LINE_HEIGHT;
            }
        }
        if(pageID == 3)
        {
            if(t->Category < ITEM_TYPE_ITEM)
            {
                drawCount++;
                if(drawCount < currentScrollStep) continue;
                if(drawCount >= (currentScrollStep + currentWindow->VisibleItemCount)) continue;
                DrawInventoryItem(x + 16, dy, t, (selectedItem  && CurrentInventoryWindow == dest)  ? currentWindow->CurrentTransferAmount: 0);
                if(selectedItem) currentWindow->CurrentItemRow = i;
                dy += INVENTORY_LINE_HEIGHT;
            }
        }
    }
    int sy = y + 16 + ((currentSelectionIndex - currentScrollStep) * 20);
    if(CurrentInventoryWindow == dest) DrawSelectionBox(x + 16, sy, 256, INVENTORY_LINE_HEIGHT);
    if(currentPageItemCount>currentWindow->VisibleItemCount) DrawHScrollBar((x + width) - 16, y + 16, currentScrollStep, currentWindow->VisibleItemCount * INVENTORY_LINE_HEIGHT, currentPageItemCount * INVENTORY_LINE_HEIGHT);
}

void DrawNameWindow(int dest, int x, int y)
{
    DrawWindowBG(x, y, 256, 76);
    DrawAsciiBar(x + INFO_BAR_OFFSET_X, y + INFO_BAR_OFFSET_Y, "CHARACTER");
    int row = (SourceMCSlot * 3) + SourceCharacterSlot;
    if(dest) row = (DestMCSlot * 3) + DestCharacterSlot;
    DrawBlitTex(row, x + 16, y + 12);
    char texBuffer[64];
    int invCount = SourceInventoryCount;
    int stoCount = SourceStorageCount;
    int gp = SourceGP;
    if(dest)
    {
        invCount = DestInventoryCount;
        stoCount = DestStorageCount;
        gp = DestGP;
    }
    sprintf(texBuffer, "GP: %d", gp);
    DrawString(x + 16, y + 32, (unsigned char*)texBuffer);

    sprintf(texBuffer, "Inventory: %d/40", invCount);
    DrawString(x + 16, y + 52, (unsigned char *)texBuffer);


    sprintf(texBuffer, "Storage: %d/99", stoCount);
    DrawString(x + 128, y + 52, (unsigned char *)texBuffer);


}


void DoGPTransfer()
{
    int * sGP = &SourceGP;
    int * dGP = &DestGP;
    if(CurrentInventoryWindow)
    {
        sGP = &DestGP;
        dGP = &SourceGP;
    }

    *sGP -= GPTransferAmount;
    *dGP += GPTransferAmount;
    GPTransferAmount = 0;
    GPDigit = 0;
    InventoriesDirty = 1;
    PopWindow();
}

int GPDigit = 0;
int GPTransferAmount = 0;
void DrawGPTransferDialog()
{
    int wWidth = 256;
    int wHeight = 32;

    int wX = (gsGlobal->Width / 2) - (wWidth / 2);
    int wY = (gsGlobal->Height / 2) - (wHeight / 2);

    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawAsciiBar(wX + INFO_BAR_OFFSET_X, wY + INFO_BAR_OFFSET_Y, "TRANSFER GP");

    DrawString(wX + 16, wY + 12, (unsigned char *)"GP:");


    int i = 0;
    char gpBuff[16];
    sprintf(gpBuff, "%08d", GPTransferAmount);
    int sWidth = GetStringWidth((unsigned char *)gpBuff);
    int dX = (wX + wWidth) - sWidth - 16;
    //int gpLen = strlen(gpBuff);
    //HighestGPAddOffset = gpLen;
    int e = 0;
    for(i = 0, e = 7; i < 8; i++, e--)
    {
        dX += DrawCharacterWithBG(dX, wY + 12, gpBuff[i], (e == GPDigit) ? 0x40000080 : 0x00808080) + 1;
        //printf("E: %d: Char: %c\n", e, gpBuff[i]);
    }
}

int DiscardCurrentSelection = 0;
void DrawDiscardConfirmationDialog()
{
    //In EVERY SINGLE timeline where I didn't code this, it was the first thing requested...
    int wWidth = 256;
    int wHeight = 128;
    int wX = (gsGlobal->Width / 2) - (wWidth / 2);
    int wY = (gsGlobal->Height / 2) - (wHeight / 2);
    int wMid = (wWidth / 2);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    //This is a hack because I can't read windows when they're overlapped and I don't feel like futzing with it any further...
    DrawAsciiBar(wX + INFO_BAR_OFFSET_X, wY + INFO_BAR_OFFSET_Y, "WARNING");

    DrawStringCentered(wX + wMid, wY + 16, (unsigned char *)"Warning, There are unsaved changes!");
    DrawStringCentered(wX + wMid, wY + 36, (unsigned char *)"Do you want to discard changes?");

    DrawString(wX + wMid - 64, wY + 78, (unsigned char*)"Yes");
    DrawString(wX + wMid + 32, wY + 78, (unsigned char*)"No");

    int bX = DiscardCurrentSelection ? 16 : -78;
    DrawSelectionBox(wX + wMid + bX, wY + 78, 48, 16);

}

void RefreshCharacters()
{

    //Get Characters for Slot 1...
    PushWindow(&DrawMC0AccessDialog_Window);
    Draw();
    EnumerateMC(0);
    if(CheckForFragmentSave(0))
    {
        EnumerateCharacters(0);
        unsigned char * slot1Names = (unsigned char *)&Slot1CharacterNames;
        BlitString(0, slot1Names);
        BlitString(1, &slot1Names[0x20]);
        BlitString(2, &slot1Names[0x40]);
    }
    else
    {
        BlitString(0, (unsigned char *)"NO SAVE FILE");
        BlitString(1, (unsigned char *)"");
        BlitString(2, (unsigned char *)"");
    }

    PopWindow();

    PushWindow(&DrawMC1AccessDialog_Window);
    Draw();
    EnumerateMC(1);
    if(CheckForFragmentSave(1))
    {
        EnumerateCharacters(1);
        unsigned char * slot1Names = (unsigned char*)&Slot2CharacterNames;
        BlitString(3, slot1Names);
        BlitString(4, &slot1Names[0x20]);
        BlitString(5, &slot1Names[0x40]);
    }
    else
    {
        BlitString(3, (unsigned char *)"NO SAVE FILE");
        BlitString(4, (unsigned char *)"");
        BlitString(5, (unsigned char *)"");
    }
    PopWindow();
}

void DrawMCIODialog(int slot)
{
    int windowX = (gsGlobal->Width / 2) - (MCDialog_WIDTH / 2);
    int windowY = (gsGlobal->Height / 2) - (MCDialog_HEIGHT / 2);

    DrawWindowBG(windowX, windowY, MCDialog_WIDTH, MCDialog_HEIGHT);
    DrawAsciiBar(windowX + INFO_BAR_OFFSET_X, windowY + INFO_BAR_OFFSET_Y, "INFO");

    sprintf(MCSlotAccesStr, "[ Memory Card Slot %d ]", slot);
    DrawStringCentered((windowX + (MCDialog_WIDTH / 2)), windowY + 16, (unsigned char *)MCSlotAccesStr);

    DrawString(windowX + 14, windowY + 38, (unsigned char *)"Accessing Memory Card...\nPlease do not remove or turn off power...");
}


void DrawMCIODialogCustom(int slot, unsigned char * text)
{
    int windowX = (gsGlobal->Width / 2) - (MCDialog_WIDTH / 2);
    int windowY = (gsGlobal->Height / 2) - (MCDialog_HEIGHT / 2);

    DrawWindowBG(windowX, windowY, MCDialog_WIDTH, MCDialog_HEIGHT);
    DrawAsciiBar(windowX + INFO_BAR_OFFSET_X, windowY + INFO_BAR_OFFSET_Y, "INFO");

    sprintf(MCSlotAccesStr, "[ Memory Card Slot %d ]", slot);
    DrawStringCentered((windowX + (MCDialog_WIDTH / 2)), windowY + 16, (unsigned char *)MCSlotAccesStr);

    sprintf(MCAccesStr, "Accessing Memory Card...\n%s\nPlease do not remove or turn off power...", text);
    DrawString(windowX + 14, windowY + 38, (unsigned char *)MCAccesStr);
}

static float ArrowSpinnies = 0;
static int ArrowSpinniesDir = 0;
void DrawCharacterDialog(unsigned char * Title, unsigned char * text)
{
    int wWidth = gsGlobal->Width - 32;
    int wHeight = (16 * 5) + (8 * 4);
    int x = (gsGlobal->Width - wWidth) / 2;
    int y = gsGlobal->Height - (wHeight + 8);

    UITile_t tl = GetTile(TILE_AlternateWindowTop_Left);
    UITile_t tm = GetTile(TILE_AlternateWindowTop_Mid);
    UITile_t tr = GetTile(TILE_AlternateWindowTop_Right);
    UITile_t tlm = GetTile(TILE_AlternateWindowMid_Left);
    UITile_t tmm = GetTile(TILE_AlternateWindowMid_Mid);
    UITile_t trm = GetTile(TILE_AlternateWindowMid_Right);

    UITile_t ml = GetTile(TILE_Window_Mid_Left);
    UITile_t mm = GetTile(TILE_Window_Mid_Mid);
    UITile_t mr = GetTile(TILE_Window_Mid_Right);

    UITile_t bl = GetTile(TILE_Window_Bottom_Left);
    UITile_t bm = GetTile(TILE_Window_Bottom_Mid);
    UITile_t br = GetTile(TILE_Window_Bottom_Right);

    int mw = wWidth - (tl.width + tr.width);
    int mh = wHeight - (tl.height + bl.height + tlm.height);
    int dy = y;
    DrawTile(x, dy, tl);
    DrawTileSized(x + tl.width, dy, mw, tm.height, tm);
    DrawTile(x + tl.width + mw, dy, tr);

    dy += tl.height;
    DrawTile(x, dy, tlm);
    DrawTileSized(x + tlm.width, dy, mw, tmm.height, tmm);
    DrawTile(x + tlm.width + mw, dy, trm);

    dy += tlm.height;
    DrawTileSized(x, dy, ml.width, mh, ml);
    DrawTileSized(x + ml.width, dy, mw, mh, mm);
    DrawTileSized(x + ml.width + mw, dy, mr.width, mh, mr);

    dy += mh;
    DrawTile(x, dy, bl );
    DrawTileSized(x + bl.width, dy, mw, bm.height, bm);
    DrawTile(x + bl.width + mw, dy, br);

    DrawString(x + 16, y + 8, Title);
    DrawString(x + 16, y + 32, text);

    //Almost forgot the Cherry...
    UITile_t arrow = GetTile(TILE_BlueArrow_Down);
    float spin = 0.7f;
    if(ArrowSpinniesDir == 0)
    {
        ArrowSpinnies += spin;
        if(ArrowSpinnies > 20.0f)ArrowSpinniesDir = 1;
    }
    else
    {
        ArrowSpinnies -= spin;
        if(ArrowSpinnies < 0.0f)ArrowSpinniesDir = 0;
    }
    int arrowWidth = (int)ArrowSpinnies;
    int dx = (x + ((wWidth / 2 ) - (arrowWidth / 2 )));
    dy = (y + wHeight - 18) + (int)(ArrowSpinnies * 0.25f);

    DrawTileSized(dx, dy, (int)ArrowSpinnies, arrow.height, arrow );
}



int CharacterSelectionIndex = 0;
void ValidateCharacterSelection()
{
    int row = CharacterSelectionIndex % 6;
    int column = CharacterSelectionIndex / 6;

    int id = (row > 2) ? row - 3 : row;
    int result = 0;

    if(column)
    {
        result = (Slot2CharacterFlags >> id) & 1;
    }
    else
    {
        result = (Slot1CharacterFlags >> id) & 1;
    }

    if(result)
    {
        if(row > 2)
        {
            DestMCSlot = column;
            DestCharacterSlot = (row - 3);
            printf("DST SLOT: %d, CHARACTER: %d\n", DestMCSlot, DestCharacterSlot);
        }
        else
        {
            SourceMCSlot = column;
            SourceCharacterSlot = row;
            printf("SRC SLOT: %d, CHARACTER: %d\n", SourceMCSlot, SourceCharacterSlot);
        }
    }

}

int CharacterSelect_MoveCursor(int currentIndex, int dir)
{
    int row = currentIndex % 6;
    int column = currentIndex / 6;
    int result = 0;


    switch (dir) {
    case SELECTION_MOVE_DIRECTION_DOWN:
        row = (row == 5) ? 0 : row + 1;
        break;
    case SELECTION_MOVE_DIRECTION_UP:
        row = (row == 0) ? 5 : row - 1;
        break;
    case SELECTION_MOVE_DIRECTION_LEFT:
    case SELECTION_MOVE_DIRECTION_RIGHT:
        column = (column == 1) ? 0 : 1;
        break;
    default:
        break;
    }

    result = (column * 6) + row;
    return result;
}

void DrawCharacterSelection()
{

    int wWidth = 384;
    int wHeight = 280; //(16 * 4) + (16 * 8);
    int wX = (gsGlobal->Width / 2) - (wWidth / 2);
    int wY = (gsGlobal->Height / 2) - (wHeight / 2);
    DrawWindowBG(wX, wY, wWidth, wHeight);
    DrawAsciiBar(wX + INFO_BAR_OFFSET_X, wY + INFO_BAR_OFFSET_Y, "CHARACTER SELECT");

    int SourceStartRow = wY + 48;
    int StartColumn = wX + 16;
    int DestStartRow = SourceStartRow + (16 * 7) + (4 * 5);

    //Dest Slot 1
    u32 nonColor = 0x80404040;
    u32 color = 0x80808080;

    int s1Flags = Slot1CharacterFlags;
    int s2Flags = Slot2CharacterFlags;

    DrawString(wX + 16, SourceStartRow - 32, (unsigned char *)"Source");
    DrawString(wX + 24, SourceStartRow - 16, (unsigned char *)"Slot 1");
    DrawString(wX  + 232, SourceStartRow - 16, (unsigned char *)"Slot 2");

    //Source Slot 1
    DrawBlitTexColor(0, StartColumn + 16, SourceStartRow + 16, (s1Flags & SLOT_CHARACTER1_FLAG) ? color: nonColor);
    DrawBlitTexColor(1, StartColumn + 16, SourceStartRow + 32 + 4, (s1Flags & SLOT_CHARACTER2_FLAG) ? color: nonColor);
    DrawBlitTexColor(2, StartColumn + 16, SourceStartRow + 48 + 8, (s1Flags & SLOT_CHARACTER3_FLAG) ? color: nonColor);
    //Source Slot 2
    DrawBlitTexColor(3, StartColumn + 224, SourceStartRow  + 16, (s2Flags & SLOT_CHARACTER1_FLAG) ? color: nonColor);
    DrawBlitTexColor(4, StartColumn + 224, SourceStartRow  + 32 + 4, (s2Flags & SLOT_CHARACTER2_FLAG) ? color: nonColor);
    DrawBlitTexColor(5, StartColumn + 224, SourceStartRow  + 48 + 8, (s2Flags & SLOT_CHARACTER3_FLAG) ? color: nonColor);

    DrawHDiv(wX + 8, DestStartRow - 54, wWidth - 16);

    DrawString(wX + 16, DestStartRow - 32, (unsigned char *)"Destination");
    DrawString(wX + 24, DestStartRow - 16, (unsigned char *)"Slot 1");
    DrawString(wX  + 232, DestStartRow - 16, (unsigned char *)"Slot 2");



    DrawBlitTexColor(0, StartColumn + 16, DestStartRow + 16, (s1Flags & SLOT_CHARACTER1_FLAG) ? color: nonColor);
    DrawBlitTexColor(1, StartColumn + 16, DestStartRow + 32 + 4, (s1Flags & SLOT_CHARACTER2_FLAG) ? color: nonColor);
    DrawBlitTexColor(2, StartColumn + 16, DestStartRow + 48 + 8, (s1Flags & SLOT_CHARACTER3_FLAG) ? color: nonColor);
    //Dest Slot 2
    DrawBlitTexColor(3, StartColumn + 224, DestStartRow + 16, (s2Flags & SLOT_CHARACTER1_FLAG) ? color: nonColor);
    DrawBlitTexColor(4, StartColumn + 224, DestStartRow + 32 + 4, (s2Flags & SLOT_CHARACTER2_FLAG) ? color: nonColor);
    DrawBlitTexColor(5, StartColumn + 224, DestStartRow + 48 + 8, (s2Flags & SLOT_CHARACTER3_FLAG) ? color: nonColor);


    int SelectionRectX = 0;
    int SelectionRectY = 0;
    int poop = 0;
    if(SourceCharacterSlot != -1)
    {
        //Draw Currently Selected Characters...
        SelectionRectX = StartColumn + (SourceMCSlot * 210);
        SelectionRectY = SourceStartRow + (4 * SourceCharacterSlot) + (16 * (SourceCharacterSlot + 1));
        DrawTileID(SelectionRectX, SelectionRectY, TILE_BlueArrow_Right);
    }
    if(DestCharacterSlot != -1)
    {
        SelectionRectX = StartColumn + (DestMCSlot* 210);
        SelectionRectY = DestStartRow + (4 * DestCharacterSlot) + (16 * (DestCharacterSlot + 1));
        DrawTileID(SelectionRectX, SelectionRectY, TILE_BlueArrow_Right);
    }

    SelectionRectX = StartColumn + 8 + ((CharacterSelectionIndex / 6) * 208);
    SelectionRectY = (CharacterSelectionIndex %  6);
    poop = SelectionRectY;
    if(poop > 2) poop -= 3;
    SelectionRectY = (((SelectionRectY > 2) ? DestStartRow: SourceStartRow) + (4 * poop) + (16 * (poop + 1))) - 2;

    DrawSelectionBox(SelectionRectX, SelectionRectY, 128, 20);

}



void DrawHelpText()
{
    float sheight = gsGlobal->Height;
    float swidth = gsGlobal->Width;
    //Draw black background
    float htY = sheight - (HelpTextHeight * 16.0f) - 6.0f;
    gsKit_prim_sprite(gsGlobal, 0.0f, htY - 8.0f, swidth, sheight, 0.0f, 0x80000000);
    DrawString(8, htY - 6, (unsigned char *)HelpText);
}

void SetHelpText(int id)
{
    if(HelpTextID != id)
    {
        switch (id) {
        case HELP_TEXT_CHARACTER_SELECT_SELECT:
            sprintf(HelpText, "Select Source and Destination Characters with DPAD + [X], Press START to Confirm\n  Press [S] to Refresh Memory Cards");
            HelpTextHeight = 2;
            break;
        case HELP_TEXT_SELECT_ITEMS:
            //sprintf(HelpText, "Use UP/DOWN to Select Items, LEFT/RIGHT to Set Amount to Transfer, Use L1/R1 to switch Tabs,\n[S] to switch active Character, START to Confirm, SELECT to Return to Character Select");
            sprintf(HelpText, "Use UP/DOWN to Select an Item, LEFT/RIGHT to Set Amount, [X] to Transfer Item\n L1/R1 To switch Tabs, [S] to switch Active Character, [T] to Transfer GP\nSTART to Save Changes and Return, SELECT to Discard Changes and Return.");
            HelpTextHeight = 3;
            break;
        case HELP_TEXT_TRANSFER_ITEMS_RESULT:
            sprintf(HelpText, "Press [X] to Continue");
            HelpTextHeight = 1;
            break;
        case HELP_TEXT_TRANSFER_GP:
            sprintf(HelpText, "Use LEFT/RIGHT to increase Digit, UP/DOWN to increase Value in current digit\nPress [X] to Confirm, [C] to Cancel");
            HelpTextHeight = 2;
            break;
        }
        HelpTextID = id;
    }
}
