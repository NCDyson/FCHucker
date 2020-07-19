#include <libpad.h>
#include <stdio.h>
#include <tamtypes.h>
#include "controller.h"
#include <kernel.h>

static char padBuff[256] __attribute__((aligned(64)));
u32 PadData = 0;

void InitPad()
{
    int ret;
    padInit(0);

    ret = padPortOpen(0, 0, padBuff);
    if(ret == 0)
    {
        printf("padOpenPort failed\n");
        SleepThread();
    }

    WaitPadReady(0, 0);
    //We only need barebones pad functionality.
}

int WaitPadReady(int port, int slot)
{
    int state;
    int lastState;

    state = padGetState(port, slot);
    lastState = -1;
    printf("Waiting for PAD...\n");
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1))
    {
        state=padGetState(port, slot);
    }

    if(lastState != -1)
    {
        printf("Pad OK!\n");
    }
    return 0;
}
static u32 oldPad;
u32 GetPad()
{
    struct padButtonStatus buttons;
    int ret = padGetState(0, 0);
    while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
    {
        if(ret==PAD_STATE_DISCONN)
        {
            printf("Controller Disconnected...\n");
        }
        ret = padGetState(0, 0);
    }

    ret = padRead(0, 0, &buttons);
    u32 paddata = 0xFFFF ^ buttons.btns;
    u32 newButtons = paddata & ~oldPad;
    oldPad = paddata;

    return newButtons;
}
