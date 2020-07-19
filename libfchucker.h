#ifndef LIBFCHUCKER_H
#define LIBFCHUCKER_H
#include <stdlib.h>
#include <stdio.h>
#include <tamtypes.h>
//These are just bit flags so we know which characters are valid

extern u8 Slot1CharacterNames[32 * 3];
extern u8 Slot2CharacterNames[32 * 3];
extern u32 Slot1CharacterFlags;
extern u32 Slot2CharacterFlags;

extern u8 * SourceInventory;
extern u8 * DestInventory;
extern int SourceInventoryCount;
extern int SourceStorageCount;
extern int DestInventoryCount;
extern int DestStorageCount;

extern int SourceGP;
extern int DestGP;

//For stuff, IDK
extern int SourceMCSlot;
extern int SourceCharacterSlot;

extern int DestMCSlot;
extern int DestCharacterSlot;

extern const char * BackupDIR;
extern const char * FragmentDIR;

#define MAIN_FILE_NAME "BISLPS-25527DOTHACK"
#define MAIN_FILE_CHECKSUM_OFFSET 0x4458
#define CHARACTER_1_FILE_NAME "dhdata01"
#define CHARACTER_2_FILE_NAME "dhdata02"
#define CHARACTER_3_FILE_NAME "dhdata03"
#define CHARACTER_FILE_CHECKSUM_OFFSET 0x36f8
#define MAIL_FILE_NAME "dhdata04"
#define MAIL_FILE_CHECKSUM_OFFSET 0x39b80
#define DISKID__FILE_NAME "dhdata05"
#define DISKID__FILE_CHECKSUM_OFFSET 0x170

#define CHARACTER_DEST 1
#define CHARACTER_SRC 0

#define INVENTORY_OFFSET 0x5c
#define INVENTORY_SIZE (4 * 40)
#define STORAGE_OFFSET 0xafc
#define STORAGE_SIZE (4 * 99)
#define INVENTORY_STORAGE_TOTAL (99 + 40)
#define GPOffset 0x87c0
#define MAX_GP 99999999

#define SLOT_CHARACTER1_FLAG 1
#define SLOT_CHARACTER2_FLAG 2
#define SLOT_CHARACTER3_FLAG 4
#define NOR(a, b) ((~a) & (~b))

int CheckForFragmentSave(int slot);
u32 EnumerateCharacters(int slot);
int ReadCharacter(int slot, int id, int dest);
int WriteCharacter(int slot, int id, int dest);

u16 Checksum(u8 * input, u32 size, u32 checksumOffset);
void EncryptSaveFile(u8 * data, int dataSize, int fileType);
void DecryptSaveFile(u8 *data, int dataSize);

void FreeInventory();
#endif // LIBFCHUCKER_H
