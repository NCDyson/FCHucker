#include "main.h"
#include "graphics.h"
#include "ui.h"
#include "UITileset.h"
#include "fonts.h"
#include "blittext.h"
#include "memcard.h"
#include "libfchucker.h"
#include "windowstack.h"
#include "controller.h"
#include "util.h"
#include "inventory.h"
unsigned char testStr1[] = {0x82, 0x63, 0x82, 0x78, 0x82, 0x72, 0x82, 0x6E, 0x6E, 0x77, 0x61, 0x73, 0x68, 0x65, 0x72, 0x65, 0x00};
unsigned char testStr2[] = {0x81, 0xA9, 0x81, 0xA4, 0x81, 0xA5, 0x81, 0xA6, 0xED, 0x40, 0x00};




int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    GraphicsInit();
    LoadModules();
    InitMC();
    InitPad();
    PrintDir(0, "/BISLPS-25527DOTHACK/*");
    PrintDir(0, "BISLPS-25527HUCKER/*");

    InitFonts(gsGlobal);

    gsKit_clear(gsGlobal, ClearColor);

    PushWindow(&DrawCharSelectScreen_Window);

    unsigned char ClearStr[] = "No Character";
    BlitString(0, ClearStr);
    BlitString(1, ClearStr);
    BlitString(2, ClearStr);
    BlitString(3, ClearStr);
    BlitString(4, ClearStr);
    BlitString(5, ClearStr);




    //First up, Load Characters...
    RefreshCharacters();


    while(1)
    {
        Draw();
    }



    return 0;
}


