#include "memcard.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

static sceMcTblGetDir mcDir[MCDIR_Count] __attribute__((aligned(64)));
static int mc1Type, mc1Free, mc1Format, mc1Status;
static int mc2Type, mc2Free, mc2Format, mc2Status;
static char MCDirPath[256] = {0};
void LoadModules()
{
    SifInitRpc(0);
    int ret = SifLoadModule("rom0:SIO2MAN", 0 , NULL);
    if(ret < 0)
    {
        printf("Failed to load Module: SIO2MAN\n");
        SleepThread();
    }

    ret = SifLoadModule("rom0:MCMAN", 0, NULL);
    if(ret < 0)
    {
        printf("Failed to load Module: MCMAN\n");
        SleepThread();
    }

    ret = SifLoadModule("rom0:MCSERV", 0, NULL);
    if(ret < 0)
    {
        printf("Failed to load Module: MCSERV\n");
        SleepThread();
    }

    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
    if(ret < 0)
    {
        printf("Failed to load Module: PADMAN\n");
        SleepThread();
    }
}

void InitMC()
{
    if(mcInit(MC_TYPE_MC) < 0)
    {
        printf("Failed to init memcard server\n");
        SleepThread();
    }
}

void WaitForMC(int slotID)
{
    EnumerateMC(slotID);
    sleep(1);
    EnumerateMC(slotID);
}

void EnumerateMC(int slotID)
{

    int retryCount = 0;

    int fakeFmt = 0;
    int * mcType = &mc1Type;
    int * mcFree = &mc1Free;
    int * mcFormat = &mc1Format;
    int * mcStatus = &mc1Status;
    if(slotID != 0)
    {
        mcType = &mc2Type;
        mcFree = &mc2Free;
        mcFormat = &mc2Format;
        mcStatus = &mc2Status;
    }

    //Hacky, but it keeps you from having to spam refresh...
    while(retryCount < 20)
    {
        int result;
        mcGetInfo(slotID, 0, mcType, mcFree, &fakeFmt);
        mcSync(0, NULL, &result);
        if(result == 0) return;

        if(result == -1)
        {
            //printf("MC%d: Formatted Memory Card Inserted...\n", slotID);
            *mcStatus = MC_VALID_FORMATTED;
            *mcFormat = MC_FORMATTED;
        }
        else if(result <= -2)
        {
            //printf("MC%d: Unformatted Memory Card Inserted...\n", slotID);
            *mcStatus = MC_VALID_UNFORMATTED;
            *mcFormat = MC_UNFORMATTED;
            retryCount++;
        }
        else if(result <= -10)
        {
            //printf("MC%d: Could not detect Memory Card...\n", slotID);
            *mcStatus = MC_INVALID;
            *mcFormat = MC_UNFORMATTED;
        }

    }
}

int ReadDir(int slot, char * path)
{
    int result;
    mcGetDir(0, slot, path, 0, MCDIR_Count, mcDir);
    mcSync(0, NULL, &result);
    return result;
}

int CheckDirExists(int slot, char * path)
{
    sceMcTblGetDir tbl[2];
    return (mcGetDir(0, slot, path, 0, 1, (sceMcTblGetDir*)&tbl) >= 0);
}

void PrintDir(int slot, char *path)
{
    int dirCount = ReadDir(slot, path);
    if(dirCount > 0)
    {
        int i = 0;
        printf("MC: %s:\n", path);
        for(i = 0; i < dirCount; i++)
        {
            if(mcDir[i].AttrFile & MC_ATTR_SUBDIR)
            {
                printf("\t[DIR] %s\n", mcDir[i].EntryName);
            }
            else
            {
                printf("\t%s\n", mcDir[i].EntryName);
            }
        }
    }
    else
    {
        printf("MC: %s does not exist.\n", path);
    }
}



int CopySaveFileContents(int slot, char *sourceDir, char *destDir)
{
    //TODO: CopySaveFileContentsTo: Doesn't handle subdirs, which isn't really a problem for fragment, but your uses may vary
    if(!CheckDirExists(slot, sourceDir))
    {
        return COPY_SAVE_FAIL_SOURCE_NOT_EXIST;
    }

    if(!CheckDirExists(slot, destDir))
    {
        sprintf(MCDirPath, "mc%d:%s", slot, destDir);
        if(mkdir(MCDirPath, 0777) < 0) return COPY_SAVE_FAIL_DEST_CREATE_FAIL;
    }

    int fCount = ReadDir(slot, sourceDir);
    int i = 0;
    int filesCopied = 0;
    for(i = 0; i <fCount; i++)
    {
        //if you choose to handle SubDirs, you need to handle . and ..
        if(mcDir[i].AttrFile & MC_ATTR_SUBDIR) continue;

        u8 * tmpFileContents = malloc(mcDir[i].FileSizeByte);
        sprintf(MCDirPath, "mc%d:%s/%s", slot, sourceDir, mcDir[i].EntryName);
        if(tmpFileContents == NULL)
        {
            printf("Error allocating memory(0x%X bytes) to read file %s...\n", mcDir[i].FileSizeByte, MCDirPath);
            return COPY_SAVE_FAIL_OTHER;
        }

        int fd = open(MCDirPath, O_RDONLY);
        if(fd <= 0)
        {
            printf("Error opening file %s\n", MCDirPath);
            free(tmpFileContents);
            return COPY_SAVE_FAIL_OTHER;
        }

        read(fd, tmpFileContents, mcDir[i].FileSizeByte);
        close(fd);

        sprintf(MCDirPath, "mc%d:%s/%s", slot, destDir, mcDir[i].EntryName);
        //int fdo = creat(MCDirPath, 0777);
        int fdo = open(MCDirPath, O_WRONLY | O_CREAT | O_TRUNC, 0755);
        if(fdo < 0)
        {
            printf("Error opening file %s for writing...\n", MCDirPath);
            free(tmpFileContents);
            return COPY_SAVE_FAIL_OTHER;
        }

        write(fdo, tmpFileContents, mcDir[i].FileSizeByte);
        close(fdo);
        printf("Copied mc%d:%s/%s to %s, 0x%X bytes", slot, sourceDir, mcDir[i].EntryName, MCDirPath, mcDir[i].FileSizeByte);
        free(tmpFileContents);
        filesCopied += 1;
    }
    return filesCopied;
}


