#define INCBIN_PREFIX
#include "incbin.h"
#include <tamtypes.h>

INCBIN(ItemDef, "ItemDef.bin");

unsigned char * TwinBlade = (unsigned char *)&ItemDefData[0x0];
int TwinBladeCount = 82;
unsigned char * BladeMaster = (unsigned char *)&ItemDefData[0xA40];
int BladeMasterCount = 76;
unsigned char * HeavyBlade = (unsigned char *)&ItemDefData[0x13C0];
int HeavyBladeCount = 97;
unsigned char * HeavyAxe = (unsigned char *)&ItemDefData[0x1FE0];
int HeavyAxeCount = 76;
unsigned char * LongArm = (unsigned char *)&ItemDefData[0x2960];
int LongArmCount = 74;
unsigned char * Wavemaster = (unsigned char *)&ItemDefData[0x32A0];
int WavemasterCount = 77;
unsigned char * Helmet = (unsigned char *)&ItemDefData[0x3C40];
int HelmetCount = 64;
unsigned char * Armor = (unsigned char *)&ItemDefData[0x4440];
int ArmorCount = 63;
unsigned char * HandArmor = (unsigned char *)&ItemDefData[0x4C20];
int HandArmorCount = 62;
unsigned char * LegArmor = (unsigned char *)&ItemDefData[0x53E0];
int LegArmorCount = 63;
unsigned char * Items = (unsigned char *)&ItemDefData[0x5BC0];
int ItemsCount = 24;
unsigned char * Scrolls = (unsigned char *)&ItemDefData[0x5EC0];
int ScrollsCount = 72;
unsigned char * SpecialItems = (unsigned char *)&ItemDefData[0x67C0];
int SpecialItemsCount = 3;
unsigned char * Treasure = (unsigned char *)&ItemDefData[0x6820];
int TreasureCount = 14;
