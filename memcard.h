#ifndef MEMCARD_H
#define MEMCARD_H
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
//#include <fileio.h>
#include <malloc.h>
#include <libmc.h>
#include <stdio.h>
#include <string.h>

#define MC_INVALID 0
#define MC_VALID_FORMATTED 1
#define MC_VALID_UNFORMATTED 2

#define MC_GETDIR_ERROR_PATH_INVALID -4
#define MC_GETDIR_ERROR_UNFORMATTED -2
#define MC_ERROR_INVALID_CARD -10
#define MCDIR_Count 64

void LoadModules();
void InitMC();
void WaitForMC(int slotID);
void EnumerateMC(int slotID);
int ReadDir(int slot, char * path);
int CheckDirExists(int slot, char * path);
void PrintDir(int slot, char * path);

#define COPY_SAVE_FAIL_SOURCE_NOT_EXIST -1
#define COPY_SAVE_FAIL_DEST_CREATE_FAIL -2
#define COPY_SAVE_FAIL_OTHER -3

/**
 * @brief CopySaveFileContents
 * @param slot MemoryCard Slot
 * @param sourceDir Directory Name, no trailing/leading slashes
 * @param destDir Directory Name, no trailing/leading slashes
 * @return Number of files copied, otherwise:
 * COPY_SAVE_FAIL_SOURCE_NOT_EXIST
 * COPY_SAVE_FAIL_DEST_CREATE_FAIL
 * COPY_SAVE_FAIL_OTHER
 */
int CopySaveFileContents(int slot, char * sourceDir, char * destDir);

#endif // MEMCARD_H
