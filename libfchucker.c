#include "libfchucker.h"
#include "memcard.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"


u8 Slot1CharacterNames[32 * 3];
u8 Slot2CharacterNames[32 * 3];
u32 Slot1CharacterFlags = 7;
u32 Slot2CharacterFlags = 7;

u8 * SourceInventory = NULL;
u8 * DestInventory = NULL;
int SourceInventoryCount = 0;
int SourceStorageCount = 0;
int DestInventoryCount = 0;
int DestStorageCount = 0;

int SourceGP = 0;
int DestGP = 0;

//For stuff, IDK
int SourceMCSlot = 0;
int SourceCharacterSlot = 0;

int DestMCSlot = 0;
int DestCharacterSlot = 0;

static char MCDIRStr[256];
#define XORPAD_SIZE 9
static u8 xorPad[XORPAD_SIZE] = {0x55, 0xc3, 0x3a, 0xcc, 0xa5, 0xaa, 0x33, 0x5a, 0x3c};
#define SWAPINDICES_SIZE 7
static u8 swapIndices[7] = {0x23, 0xd, 0x17, 0x1, 0x1f, 0x29, 0x13};

//Someday, I suppose
//const char * BackupDIR = "BISLPS-25527HUCKER";
const char * FragmentDIR= "BISLPS-25527DOTHACK";

int CheckForFragmentSave(int slot)
{
    sprintf(MCDIRStr, "mc%d:%s/icon.sys", slot, FragmentDIR);
    int fd = open(MCDIRStr, O_RDONLY);
    if(fd <= 0)
    {
        if(slot == 0)
        {
            Slot1CharacterFlags = 0;
        }
        else
        {
            Slot2CharacterFlags = 0;
        }
        if(SourceMCSlot == slot)
        {
            SourceMCSlot = -1;
            SourceCharacterSlot = -1;
        }
        if(DestMCSlot == slot)
        {
            DestMCSlot = -1;
            DestCharacterSlot = -1;
        }
        return 0;
    }
    else
    {
        close(fd);
        return 1;
    }
    return 0;

}

u32 EnumerateCharacters(int slot)
{
    u32 * SlotCharacterFlags = NULL;
    u8 * slot1Name = NULL;
    u8 * slot2Name = NULL;
    u8 * slot3Name = NULL;
    if(slot == 0)
    {
        slot1Name = &Slot1CharacterNames[0];
        SlotCharacterFlags = &Slot1CharacterFlags;
    }
    else
    {
        slot1Name = &Slot2CharacterNames[0];
        SlotCharacterFlags = &Slot2CharacterFlags;
    }
    slot2Name = &slot1Name[0x20];
    slot3Name = &slot1Name[0x40];

    //Get main file
    sprintf(MCDIRStr, "mc%d:%s/%s", slot, FragmentDIR, FragmentDIR);
    int fd = open(MCDIRStr, O_RDONLY);
    if(fd <= 0)
    {

        return -1;
    }

    u32 fSize = (u32)lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    u8 * data = malloc(fSize);
    if(data == NULL)
    {
        close(fd);
        return -1;
    }

    read(fd, data, fSize);
    close(fd);

    DecryptSaveFile(data, fSize);

    u8 * Char1 = &data[0xc];
    u8 * Char2 = &data[0x54];
    u8 * Char3 = &data[0x9c];
    *SlotCharacterFlags = 0;
    if(*((u32*)Char1) != 0)
    {
        //Char valid, maybe, copy the name
        memcpy(slot1Name, Char1 + 4, 16);
        slot1Name[16] = 0;
        *SlotCharacterFlags |= 1;
        if(SourceMCSlot == -1) SourceMCSlot = slot;
        if(SourceCharacterSlot == -1) SourceCharacterSlot = 0;
        if(DestMCSlot == -1) DestMCSlot = slot;
        if(DestCharacterSlot == -1) DestCharacterSlot = 0;
    }
    else
    {
        //*slot1Name = "No Character";
        sprintf((char *)slot1Name,"No Character");
        *SlotCharacterFlags &= 0xFFFFFFFE;
    }

    if(*((u32*)Char2) != 0)
    {
        memcpy(slot2Name, (Char2 + 4), 16);
        slot2Name[16] = 0;
        *SlotCharacterFlags |= 2;
        if(SourceMCSlot == -1) SourceMCSlot = slot;
        if(SourceCharacterSlot == -1) SourceCharacterSlot = 1;
        if(DestMCSlot == -1) DestMCSlot = slot;
        if(DestCharacterSlot == -1) DestCharacterSlot = 1;
    }
    else
    {
        //slot2Name = "No Character";
        sprintf((char * )slot2Name,"No Character");
        *SlotCharacterFlags &= 0xFFFFFFFD;
    }

    if(*((u32 *)Char3) != 0)
    {
        memcpy(slot3Name, (Char3 + 4), 16);
        slot3Name[16] = 0;
        *SlotCharacterFlags |= 4;
        //printf("Slot %d, Character 3: %s\n", slot, slot3Name);
        if(SourceMCSlot == -1) SourceMCSlot = slot;
        if(SourceCharacterSlot == -1) SourceCharacterSlot = 1;
        if(DestMCSlot == -1) DestMCSlot = slot;
        if(DestCharacterSlot == -1) DestCharacterSlot = 1;
    }
    else
    {
        //*slot3Name = "No Character";
        sprintf((char *)slot3Name,"No Character");
        *SlotCharacterFlags &= 0xFFFFFFFB;
    }

    free(data);
    return *SlotCharacterFlags;
}

int ReadCharacter(int slot, int id, int dest)
{
    sprintf(MCDIRStr, "mc%d:%s/dhdata0%d", slot, FragmentDIR, id + 1);
    int fd = open(MCDIRStr, O_RDONLY);
    if(fd <= 0)
    {

        return -1;
    }

    u32 fSize = (u32)lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    u8 * data = malloc(fSize);
    if(data == NULL)
    {
        close(fd);
        return -1;
    }

    read(fd, data, fSize);
    close(fd);

    DecryptSaveFile(data, fSize);

    u8 * inventory = malloc(INVENTORY_SIZE + STORAGE_SIZE);
    memcpy(inventory, data + INVENTORY_OFFSET, INVENTORY_SIZE);
    memcpy(inventory + INVENTORY_SIZE, data + STORAGE_OFFSET, STORAGE_SIZE);

    //int gp = *(int *)(&data + GPOffset);
    int * money = &SourceGP;
    if(dest) money = &DestGP;

    *money = *(int*)(data + GPOffset);

    free(data);
    if(dest)
    {
        if(DestInventory) free(DestInventory);
        DestInventory = inventory;

    }
    else
    {
        if(SourceInventory) free(SourceInventory);
        SourceInventory = inventory;
    }

    return 0;
}

int WriteCharacter(int slot, int id, int dest)
{
    sprintf(MCDIRStr, "mc%d:%s/dhdata0%d", slot, FragmentDIR, id + 1);
    int fd = open(MCDIRStr, O_RDONLY);
    if(fd <= 0)
    {

        return -1;
    }

    u32 fSize = (u32)lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    u8 * data = malloc(fSize);
    if(data == NULL)
    {
        close(fd);
        return -1;
    }

    read(fd, data, fSize);
    close(fd);

    DecryptSaveFile(data, fSize);

    u8 * inventory = SourceInventory;
    if(dest) inventory = DestInventory;

    memcpy(data + INVENTORY_OFFSET, inventory, INVENTORY_SIZE);
    memcpy(data + STORAGE_OFFSET, inventory + INVENTORY_SIZE, STORAGE_SIZE);

    int * money = &SourceGP;
    if(dest) money = &DestGP;

    //*money = *(int*)(data + GPOffset);
    int * dGP = (int *)(data + GPOffset);
    *dGP = *money;

    EncryptSaveFile(data, fSize, slot + 1);

    sprintf(MCDIRStr, "mc%d:%s/dhdata0%d", slot, FragmentDIR, id + 1);
    fd = open(MCDIRStr, O_WRONLY);
    if(fd <= 0)
    {
        //Well shit...
        free(data);
        return -1;
    }

    write(fd, data, fSize);
    free(data);
    if(dest)
    {
        free(DestInventory);
        DestInventory = NULL;
    }
    else
    {
        free(SourceInventory);
        SourceInventory = NULL;
    }
    return 1;
}

u16 Checksum(u8 *input, u32 size, u32 checksumOffset)
{
    u16 * checksumField = (u16*)(input + checksumOffset);
    //u16 previousChecksum = *checksumField;

    *checksumField = 0;
    u32 curSum = 0;
    u32 i = 0;
    for(i = 0; i < (size-1); i++)
    {
        curSum += (u8)input[i];
        curSum &= 0xFFFF;
    }

    return curSum;
}

void EncryptSaveFile(u8 *data, int dataSize, int fileType)
{
    int checksumOffset = 0;
    switch (fileType) {
    case 1:
    case 2:
    case 3:
        checksumOffset = CHARACTER_FILE_CHECKSUM_OFFSET;
        break;
    case 4:
        checksumOffset = MAIL_FILE_CHECKSUM_OFFSET;
        break;
    case 5:
        checksumOffset = DISKID__FILE_CHECKSUM_OFFSET;
    case 0:
    default:
        checksumOffset = MAIN_FILE_CHECKSUM_OFFSET;
    }

    //Do checksum real quick...
    u16 * checksumField = (u16 *)(data + checksumOffset);
    *checksumField = 0;

    u32 curSum = 0;
    int i = 0;
    for(i = 0; i < (dataSize - 1); i++)
    {
        curSum += (u8)data[i];
        curSum &= 0xFFFF;
    }
    *checksumField = curSum;

    //stage 1, NOR data
    i = 0;
    for(i = 0; i < (dataSize - 1); i++)
    {
        data[i] = NOR(data[i], 0);
    }

    //stage 2, generate seed
    u32 seed = 0;
    for(i = 0; i < (dataSize - 1); i++)
    {
        u32 tmpSeed = (xorPad[i % XORPAD_SIZE] ^ data[i]) & 0xff;
        seed = (seed + tmpSeed) & 0xFF;
    }

    //stage 3, swap and xor data
    for(i = 0; i < dataSize - 1; i++)
    {
        u32 curSeed = i + seed;
        u8 swap1 = data[i];
        u32 swapIndex = (swapIndices[curSeed % SWAPINDICES_SIZE] + (seed & 0x3f) + i) % (dataSize - 1);
        u8 swap2 = data[swapIndex];
        data[i] = swap2 ^ (xorPad[curSeed % XORPAD_SIZE]);
        data[swapIndex] = swap1;
    }

    //stage 4, shift data and place seed
    if((dataSize - 1) > 0x46)
    {
        for(i = (dataSize - 1); i > 0x45; i--)
        {
            data[i] = data[i - 1];
        }
        data[0x45] = seed ^ xorPad[1];
    }

}

void DecryptSaveFile(u8 *data, int dataSize)
{
    //step 1, read seed, set placeholder to 0
    u32 seed = (xorPad[1] ^ data[0x45]) & 0xFF;

    //step 2, shift data
    int i = 0;
    for(i = 0x45; i < (dataSize - 1); i++)
    {
        data[i] = data[i + 1];
    }

    //stage 3, xor data and swap it
    for(i = (dataSize - 2); i > -1; i--)
    {
        u32 curSeed = i + seed;
        u8 swap1 = data[i] ^ xorPad[curSeed % XORPAD_SIZE];
        u32 swapIndex = (swapIndices[curSeed % SWAPINDICES_SIZE] + (seed & 0x3f) + i ) % (dataSize - 1);
        u8 swap2 = data[swapIndex];
        data[i] = swap2;
        data[swapIndex] = swap1;
    }

    //stage 3, episode 1, set placehold for seed to 0
    data[dataSize - 1] = 0;

    //stage 4, NOR data
    for(i = 0; i < (dataSize - 1); i++)
    {
        data[i] = NOR(data[i], 0);
    }
}

void FreeInventory()
{
    if(SourceInventory)
    {
        free(SourceInventory);
        SourceInventory = NULL;
    }

    if(DestInventory)
    {
        free(DestInventory);
        DestInventory = NULL;
    }
}
