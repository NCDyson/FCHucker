#ifndef UI_H
#define UI_H

#include "UITex.h"
#include "graphics.h"
#include "fonts.h"
#include "blittext.h"
#include "inventory.h"

typedef struct UITile
{
    short width;
    short height;
    short x;
    short y;
}UITile_t;

/*Utility Functions*/
UITile_t GetTile(int tileID);
void PrintTileInfo(const char * name, UITile_t tile);
void DrawTileID(int x, int y, int TILEID);
void DrawTileSizedID(int x, int y, int width, int height, int TILEID);
void DrawTile(int x, int y, UITile_t tile);
void DrawTileSized(int x, int y, int width, int height, UITile_t tile);

void DrawTileF(float x, float y, UITile_t tile);
void DrawTileSizedF(float x, float y, float width, float height, UITile_t tile);


/*Widget Drawing*/
void DrawWindowBG(int x, int y, int width, int height);
#define INFO_BAR_OFFSET_X -7
#define INFO_BAR_OFFSET_Y -9
void DrawAsciiBar(int x, int y, char * text);
void DrawHDiv(int x, int y, int width);
void DrawSelectionBox(int x, int y, int width, int height);
void DrawHScrollBar(int x, int y, int step, int visibleAreaHeight, int fullAreaHeight);
#define INVENTORY_SRC_X 26
#define INVENTORY_DST_X 280
#define INVENTORY_WINDOW_WIDTH 300
#define INVENTORY_LINE_HEIGHT 20
#define INVENTORY_ITEM_COUNT 12
#define INVENTORY_WINDOW_HEIGHT ((INVENTORY_ITEM_COUNT * INVENTORY_LINE_HEIGHT) + 32)
//#define INVENTORY_WINDOW_HEIGHT 352
struct InventoryWindow_t
{
    int CurrentPage;

    int Page0Index; //Items
    int Page1Index; //Scrolls
    int Page2Index; //Treasure
    int Page3Index; //Equipment
    int Page4Index; //Items(Storage)
    int Page5Index; //Scrolls(Storage)
    int Page6Index; //Treasure(Storage)
    int Page7Index; //Equipment(Storage)

    int Page0Count;
    int Page1Count;
    int Page2Count;
    int Page3Count;
    int Page4Count;
    int Page5Count;
    int Page6Count;
    int Page7Count;

    int Page0ScrollStep;
    int Page1ScrollStep;
    int Page2ScrollStep;
    int Page3ScrollStep;
    int Page4ScrollStep;
    int Page5ScrollStep;
    int Page6ScrollStep;
    int Page7ScrollStep;

    int VisibleItemCount;
    int StorageCount;
    int InventoryCount;
    int CurrentItemRow;
    int CurrentTransferAmount;
    struct InventoryItem_t * Inventory;
    //struct ItemTransferRequest * TransferReqs;
};
typedef struct InventoryWindow_t InventoryWindow_t;

void GoBackToCharacterSelect();
void SaveAndGoBackToCharacterSelect();

void DoItemTransfer();
void CloseTransferResultWindow();
void DrawInventoryTransferResult();
void SetInventoryWindowCounts(InventoryWindow_t * window);

void GoInventoryView();
void InitInventoryWindow(InventoryWindow_t *window, int dest);

extern InventoryWindow_t InventoryWindows[2];
extern int CurrentInventoryWindow;
void DrawInventoryItem(int x, int y, struct InventoryItem_t * item , int transferAmount);
void DrawTabStrip(int x, int y, int tabCount, int currentTab, unsigned char *currentTabText);
void DrawInventoryWindow(int dest, int x, int y, int width, int height);
void DrawNameWindow(int dest, int x, int y);
/*Memory Card Access Stuff*/
#define MCDialog_WIDTH 306
#define MCDialog_HEIGHT 146
void DrawMCIODialog(int slot);
void DrawMCIODialogCustom(int slot, unsigned char *text);
void RefreshCharacters();

/*Character Selection Screen Stuff*/
#define SELECTION_MOVE_DIRECTION_DOWN 0
#define SELECTION_MOVE_DIRECTION_UP 1
#define SELECTION_MOVE_DIRECTION_RIGHT 2
#define SELECTION_MOVE_DIRECTION_LEFT 3
extern int CharacterSelectionIndex;
void ValidateCharacterSelection();
int CharacterSelect_MoveCursor(int currentIndex, int dir);
void DrawSelectionBox(int x, int y, int width, int height);
void DrawCharacterSelection();
void DrawCharacterDialog(unsigned char * Title, unsigned char * text);

extern int GPDigit;
extern int GPTransferAmount;
void DoGPTransfer();
void DrawGPTransferDialog();

extern int DiscardCurrentSelection;
void DrawDiscardConfirmationDialog();
/*Help Screen Stuff*/
#define HELP_TEXT_CHARACTER_SELECT_SELECT 0
#define HELP_TEXT_CHARACTER_SELECT_CONFIRM 1
#define HELP_TEXT_SELECT_ITEMS 2
#define HELP_TEXT_TRANSFER_ITEMS_RESULT 3
#define HELP_TEXT_TRANSFER_GP 4
void SetHelpText(int id);
void DrawHelpText();

#endif // UI_H
