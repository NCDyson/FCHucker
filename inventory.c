#define INCBIN_PREFIX
#include "incbin.h"
#include <tamtypes.h>


#include "inventory.h"
#include "libfchucker.h"
#include "codex.h"

int InventoriesDirty = 0;

char * GetTransferErrorMessage(int resultCode)
{
    if(resultCode == 0)
    {
        return "Success";
    }

    switch (resultCode & 3) {
        case 1:
            return "No Free Inventory Slots";
        case 2:
            return "No Free Storage Slots";
        case 3:
            return "No Free Inventory or Storage Slots";
    }

    if(resultCode == TRANSFER_MAX_ONHAND_STORAGE) return "Inventory and Storage counts at max capacity (99 each)";

    return "I dunno.";
}

void SetInventoriesClean()
{
    InventoriesDirty = 0;
}

void EnumerateInventory(int dest)
{
    u8 * Inventory = SourceInventory;
    int * srcCount = &SourceInventoryCount;
    int * srcStorage = &SourceStorageCount;
    if(dest)
    {
        Inventory = DestInventory;
        srcCount = &DestInventoryCount;
        srcStorage = &DestStorageCount;
    }

    int invCount = 0;
    int strCount = 0;
    int i;
    for(i = 0; i < 40; i++)
    {
        InventoryItem_t * item = (InventoryItem_t *)&Inventory[i * 4];
        if(item->ItemID != ITEM_NONE) invCount++;
    }

    for(i = 0; i < 99; i++)
    {
        InventoryItem_t * item = (InventoryItem_t *)&Inventory[INVENTORY_SIZE + (i * 4)];
        if(item->ItemID != ITEM_NONE) strCount++;
    }

    *srcCount = invCount;
    *srcStorage = strCount;
}


int GetNextOrCurrentInventorySlot(int dest, int itemCategory, int itemID, int * nextFree)
{
    int freeID = -1;
    int currentID = -1;
    int i = 0;
    InventoryItem_t * inventory = (InventoryItem_t*)SourceInventory;
    if(dest) inventory = (InventoryItem_t*)DestInventory;
    for(i = 0; i < 40; i++)
    {
        InventoryItem_t * item = &inventory[i];
        //printf("INVENTORY %d: %02X%02X%04X\n", i, item->Count, item->Category, item->ItemID);
        if((item->Category == itemCategory) && (item->ItemID == itemID)) currentID = i;
        if((item->ItemID == ITEM_NONE) && (item->Category == 0xFF) && (freeID == -1)) freeID = i;
    }

    *nextFree = freeID;
    return currentID;
}

int GetNextOrCurrentStorageSlot(int dest, int itemCategory, int itemID, int * nextFree)
{
    int freeID = -1;
    int currentID = -1;
    int i = 0;
    InventoryItem_t * inventory = (InventoryItem_t*)SourceInventory;
    if(dest) inventory = (InventoryItem_t*)DestInventory;
    for(i = 40; i < 139; i++)
    {
        InventoryItem_t * item = &inventory[i];
        if((item->Category == itemCategory) && (item->ItemID == itemID)) currentID = i;
        if((item->ItemID == ITEM_NONE) && (item->Category == 0xFF) && (freeID == -1)) freeID = i;
    }

    *nextFree = freeID - 40;
    return currentID - 40;
}

ItemTransferResult_t TransferItem(int inventorySlot, int dest, int transferAmount)
{
    ItemTransferResult_t result;
    result.inventoryCount = 0;
    result.storageCount = 0;
    result.remaining = 0;
    result.resultFlags = 0;
    result.totalLeft = 0;

    InventoryItem_t * SInventory = (InventoryItem_t *)SourceInventory;
    InventoryItem_t * DInventory = (InventoryItem_t *)DestInventory;
    if(dest)
    {
        SInventory = (InventoryItem_t *)DestInventory;
        DInventory = (InventoryItem_t *)SourceInventory;
    }

    InventoryItem_t * SourceItem = &SInventory[inventorySlot];
    result.itemID = SourceItem->ItemID;
    result.itemCategory = SourceItem->Category;

    //Try transfering to On Hand inventory first...
    int nextFreeSlot = -1;
    int currentSlot = GetNextOrCurrentInventorySlot(dest ^ 1, SourceItem->Category, SourceItem->ItemID, &nextFreeSlot);
    InventoryItem_t * DestItem = NULL;
    int canTransferToInventory = 0;
    int destSlot = -1;
    if(currentSlot >= 0)
    {
        DestItem = &DInventory[currentSlot];
        canTransferToInventory = 1;
        destSlot = currentSlot;
    }
    else
    {
        if(nextFreeSlot != -1)
        {
            DestItem = &DInventory[nextFreeSlot];
            canTransferToInventory = 1;
            destSlot = nextFreeSlot;
        }
        else
        {
            result.resultFlags |= TRANSFER_NO_FREE_ONHAND;
        }
    }

    if(canTransferToInventory)
    {
        if((DestItem->Count + transferAmount) > 99)
        {
            int remainder = (DestItem->Count + transferAmount) - 99;
            DestItem->Count = 99;
            result.inventoryCount = (transferAmount - remainder);
            transferAmount = remainder;
        }
        else
        {
            DestItem->Count += transferAmount;
            result.inventoryCount = transferAmount;
            transferAmount = 0;
        }
        DestItem->ItemID = SourceItem->ItemID;
        DestItem->Category = SourceItem->Category;
    }

    //Now check if there's anything left to go into storage...
    if(transferAmount > 0)
    {
        currentSlot = GetNextOrCurrentStorageSlot(dest ^ 1, SourceItem->Category, SourceItem->ItemID, &nextFreeSlot);
        int canTransferToStorage = 0;
        if(currentSlot >= 0)
        {
            DestItem = &DInventory[40 + currentSlot];
            canTransferToStorage = 1;
        }
        else
        {
            if(nextFreeSlot != -1)
            {
                DestItem = &DInventory[40 + nextFreeSlot];
                canTransferToStorage = 1;
            }
            else
            {
                result.resultFlags |= TRANSFER_NO_FREE_STORAGE;
            }
        }

        if(canTransferToStorage)
        {
            if((DestItem->Count + transferAmount) > 99)
            {
                int remaineder = (DestItem->Count + transferAmount) - 99;
                DestItem->Count = 99;
                result.storageCount = transferAmount - remaineder;
                transferAmount = remaineder;
            }
            else
            {
                DestItem->Count += transferAmount;
                result.storageCount = transferAmount;
                transferAmount = 0;
            }
            DestItem->ItemID = SourceItem->ItemID;
            DestItem->Category = SourceItem->Category;
        }
    }

    result.remaining = transferAmount;
    if(result.remaining > 0)
    {
        if((result.resultFlags & 3) == 0)
        {
            result.resultFlags |= TRANSFER_MAX_ONHAND_STORAGE;
        }
    }
    int totalTransfered = result.storageCount + result.inventoryCount;
    if(totalTransfered > 0)
    {
        if(totalTransfered == SourceItem->Count)
        {
            SourceItem->Count = 0;
            SourceItem->ItemID = ITEM_NONE;
            SourceItem->Category = 0xFF;
            result.totalLeft = 0;
        }
        else
        {
            SourceItem->Count -= totalTransfered;
            result.totalLeft = SourceItem->Count;
        }
    }

    if(totalTransfered != 0)
    {
        InventoriesDirty = 1;
    }

    return result;
}

unsigned char *GetItemName(int category, int id)
{
    unsigned char * result = (unsigned char*)"INVALID ITEM";
    int * itemListCount = 0;
    unsigned char * itemList = NULL;
    switch (category) {
    case ITEM_TYPE_TWINBLADE:
        itemList = TwinBlade;
        itemListCount = &TwinBladeCount;
        break;
    case ITEM_TYPE_BLADEMASTER:
        itemList = BladeMaster;
        itemListCount = &BladeMasterCount;
        break;
    case ITEM_TYPE_HEAVYBLADE:
        itemList = HeavyBlade;
        itemListCount = &HeavyBladeCount;
        break;
    case ITEM_TYPE_HEAVEAXE:
        itemList = HeavyAxe;
        itemListCount = &HeavyAxeCount;
        break;
    case ITEM_TYPE_LONGARM:
        itemList = LongArm;
        itemListCount = &LongArmCount;
        break;
    case ITEM_TYPE_WAVEMASTER:
        itemList = Wavemaster;
        itemListCount = &WavemasterCount;
        break;
    case ITEM_TYPE_HELMET:
        itemList = Helmet;
        itemListCount = &HelmetCount;
        break;
    case ITEM_TYPE_ARMOR:
        itemList = Armor;
        itemListCount = &ArmorCount;
        break;
    case ITEM_TYPE_HANDARMOR:
        itemList = HandArmor;
        itemListCount = &HandArmorCount;
        break;
    case ITEM_TYPE_LEGARMOR:
        itemList = LegArmor;
        itemListCount = &LegArmorCount;
        break;
    case ITEM_TYPE_ITEM:
        itemList = Items;
        itemListCount = &ItemsCount;
        break;
    case ITEM_TYPE_SCROLLS:
        itemList = Scrolls;
        itemListCount = &ScrollsCount;
        break;
    case ITEM_TYPE_SPECIAL_USE:
        itemList = SpecialItems;
        itemListCount = &SpecialItemsCount;
        break;
    case ITEM_TYPE_TREASURE:
        itemList = Treasure;
        itemListCount = &TreasureCount;
        break;
    default:
        break;
    }

    if(itemList != NULL)
    {
        if(id < *itemListCount)
        {
            result = &itemList[id * 0x20];
        }
    }

    return result;
}

int GetTotalAvailableCapacity(int dest, int categoryID, int itemID)
{
    int totalQty = 0;
    InventoryItem_t * inv = (InventoryItem_t *)&SourceInventory;
    if(dest) inv = (InventoryItem_t *)&DestInventory;

    //Search on hand
    int i = 0;
    for(i = 0; i < 40; i++)
    {
        InventoryItem_t * t = inv + i;
        if(t->Category == categoryID && t->ItemID == itemID)
        {
            totalQty += t->Count;
        }
    }

    //Search Storage
    for(i = 40; i < 139; i++)
    {
        InventoryItem_t * t = inv + i;
        if(t->Category == categoryID && t->ItemID == itemID)
        {
            totalQty += t->Count;
        }
    }


    return 198 - totalQty;
}
