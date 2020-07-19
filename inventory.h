#ifndef INVENTORY_H
#define INVENTORY_H
#include <tamtypes.h>
#include "ui.h"

struct InventoryItem_t
{
    u16 ItemID;
    u8 Category;
    u8 Count;
};

typedef struct InventoryItem_t InventoryItem_t;
#define TRANSFER_NO_FREE_ONHAND 1
#define TRANSFER_NO_FREE_STORAGE 2
#define TRANSFER_NO_FREE_ONHAND_STORAGE 3

#define TRANSFER_MAX_ONHAND_STORAGE 4

extern char TransferErrorMessage[];

typedef struct ItemTransferResult
{
    int inventoryCount;
    int storageCount;
    int remaining;
    int itemID;
    int resultFlags;
    int totalLeft;
    int itemCategory;
}ItemTransferResult_t;

typedef struct ItemTransferRequest
{
    int amount;
}ItemTransferRequest_t;

#define ITEM_NONE 0xFFFF
#define ITEM_TYPE_TWINBLADE 0
#define ITEM_TYPE_BLADEMASTER 1
#define ITEM_TYPE_HEAVYBLADE 2
#define ITEM_TYPE_HEAVEAXE 3
#define ITEM_TYPE_LONGARM 4
#define ITEM_TYPE_WAVEMASTER 5
#define ITEM_TYPE_HELMET 6
#define ITEM_TYPE_ARMOR 7
#define ITEM_TYPE_HANDARMOR 8
#define ITEM_TYPE_LEGARMOR 9
#define ITEM_TYPE_ITEM 10
#define ITEM_TYPE_SCROLLS 11
#define ITEM_TYPE_SPECIAL_USE 12
#define ITEM_TYPE_TREASURE 13

extern int InventoriesDirty;
void SetInventoriesClean();
char * GetTransferErrorMessage(int resultCode);

unsigned char * GetItemName(int category, int id);

void EnumerateInventory(int dest);
int GetNextOrCurrentInventorySlot(int dest, int itemCategory, int itemID, int * nextFree);
int GetNextOrCurrentStorageSlot(int dest, int itemCategory, int itemID, int * nextFree);

/**
 * @brief TransferItem
 * @param dest 0: Source -> Dest, 1: Dest -> Source
 * @param transfer: ItemTransferRequest_t containing sender inventory slot and amount to send
 */
ItemTransferResult_t TransferItem(int inventorySlot, int dest, int transferAmount);

int GetTotalAvailableCapacity(int dest, int categoryID, int itemID);


#endif // INVENTORY_H
