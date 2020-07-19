#include "blittext.h"
#include "fonts.h"
#include "graphics.h"



int BlitLineWidths[16] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

void CopyRect(int srcX, int srcY, int dstX, int dstY, int width, int height, u8 * src, u8 * dst, int srcImgWidth, int dstImgWidth)
{
    int csx = srcX;
    int csy = srcY;
    int cdx = dstX;
    int cdy = dstY;

    int y = 0;
    int x = 0;
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < (width); x++)
        {
            int srcOffset = (csy * (srcImgWidth / 2)) + (csx / 2);
            int dstOffset = (cdy * (dstImgWidth / 2)) + (cdx / 2);
            u8 s = src[srcOffset];
            u8 d = dst[dstOffset];

            int ss = (csx & 1) << 2; //Src Shift
            int ds = (cdx & 1) << 2; //Dst Shift
            int dms = ((cdx & 1) ^ 1) << 2; //dst mask shift
            u8 so = ((s >> ss) & 0xf) << ds;
            u8 doo = (d & (0xf << dms));

            dst[dstOffset] = doo | so;
            csx++;
            cdx++;
        }
        csy++;
        cdy++;
        csx = srcX;
        cdx = dstX;
    }
}

int ccKanjiStrLen(unsigned char * text)
{
    int result = 0;
    int running = 1;
    int d = 0;
    while(running)
    {
        u8 c = text[d];
        if(c == 0)
        {
            running = 0;
            break;
        }

        if(((c > 0x7e) && (c < 0xa0)) || (c > 0xdf))
        {
            result += 1;
            d += 2;
            continue;
        }
        else
        {
            result += 1;
            d += 1;
            continue;
        }
        if(d > 2048)
        {
            printf("ERROR: ccKanjiStrLen fell through 2048 bytes of text...\n");
            return 0;
        }
    }

    return result;
}

void TileIDtoXY(int tileID, int * x, int * y)
{
    *x = ((tileID % 16) * 14);
    *y = ((tileID / 16) * 16);
}

void GetSJISCharXY(int charID, int * x, int * y)
{
    //Check for REMAPS
    //There may be other ones, but I don't really care right now...
    if(charID == 0xfa58) charID = 0x878a;
    if(charID == 0xfa59) charID = 0x8782;
    if(charID == 0xfa4a) charID = 0x8774;
    if(charID == 0xfa5b) charID = 0x879a;
    if((charID >= 0xfa5c) && (charID < 0xfa8c))
    {
            charID -= 0xd1c;
    }

    //printf("CHECKING SJIS %04X\n", charID);
    //Probably a more efficient way to do this, but meh
    //0x8140 to 0x81FC TileStart 0
    if (charID >= 0x8140 && charID <= 0x81FC)
    {
        int tileDelta = charID - 0x8140;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 0, x, y);
        return;
    }
    //0x824F to 0x8258 TileStart 192
    else if (charID >= 0x824F && charID <= 0x8258)
    {
        int tileDelta = charID - 0x824F;
        TileIDtoXY(tileDelta + 192, x, y);
        return;
    }
    //0x8260 to 0x8279 TileStart 208
    else if (charID >= 0x8260 && charID <= 0x8279)
    {
        int tileDelta = charID - 0x8260;
        TileIDtoXY(tileDelta + 208, x, y);
        return;
    }
    //0x8281 to 0x829A TileStart 240
    else if (charID >= 0x8281 && charID <= 0x829A)
    {
        int tileDelta = charID - 0x8281;
        TileIDtoXY(tileDelta + 240, x, y);
        return;
    }
    //0x829F to 0x82F1 TileStart 272
    else if (charID >= 0x829F && charID <= 0x82F1)
    {
        int tileDelta = charID - 0x829F;
        TileIDtoXY(tileDelta + 272, x, y);
        return;
    }
    //0x8340 to 0x8396 TileStart 368
    else if (charID >= 0x8340 && charID <= 0x8396)
    {
        int tileDelta = charID - 0x8340;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 368, x, y);
        return;
    }
    //0x839F to 0x83B6 TileStart 464
    else if (charID >= 0x839F && charID <= 0x83B6)
    {
        int tileDelta = charID - 0x839F;
        TileIDtoXY(tileDelta + 464, x, y);
        return;
    }
    //0x83BF to 0x83D6 TileStart 496
    else if (charID >= 0x83BF && charID <= 0x83D6)
    {
        int tileDelta = charID - 0x83BF;
        TileIDtoXY(tileDelta + 496, x, y);
        return;
    }
    //0x8440 to 0x8460 TileStart 528
    else if (charID >= 0x8440 && charID <= 0x8460)
    {
        int tileDelta = charID - 0x8440;
        TileIDtoXY(tileDelta + 528, x, y);
        return;
    }
    //0x8470 to 0x8491 TileStart 576
    else if (charID >= 0x8470 && charID <= 0x8491)
    {
        int tileDelta = charID - 0x8470;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 576, x, y);
        return;
    }
    //0x849F to 0x84BE TileStart 624
    else if (charID >= 0x849F && charID <= 0x84BE)
    {
        int tileDelta = charID - 0x849F;
        TileIDtoXY(tileDelta + 624, x, y);
        return;
    }
    //0x8740 to 0x879C TileStart 656
    else if (charID >= 0x8740 && charID <= 0x879C)
    {
        int tileDelta = charID - 0x8740;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 656, x, y);
        return;
    }
    //0x889F to 0x88FC TileStart 752
    else if (charID >= 0x889F && charID <= 0x88FC)
    {
        int tileDelta = charID - 0x889F;
        TileIDtoXY(tileDelta + 752, x, y);
        return;
    }
    //0x8740 to 0x87FC TileStart 846
    else if (charID >= 0x8740 && charID <= 0x87FC)
    {
        int tileDelta = charID - 0x8740;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 846, x, y);
        return;
    }
    //0x8840 to 0x88FC TileStart 1034
    else if (charID >= 0x8840 && charID <= 0x88FC)
    {
        int tileDelta = charID - 0x8840;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1034, x, y);
        return;
    }
    //0x8940 to 0x89FC TileStart 1222
    else if (charID >= 0x8940 && charID <= 0x89FC)
    {
        int tileDelta = charID - 0x8940;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1222, x, y);
        return;
    }
    //0x8A40 to 0x8AFC TileStart 1410
    else if (charID >= 0x8A40 && charID <= 0x8AFC)
    {
        int tileDelta = charID - 0x8A40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1410, x, y);
        return;
    }
    //0x8B40 to 0x8BFC TileStart 1598
    else if (charID >= 0x8B40 && charID <= 0x8BFC)
    {
        int tileDelta = charID - 0x8B40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1598, x, y);
        return;
    }
    //0x8C40 to 0x8CFC TileStart 1786
    else if (charID >= 0x8C40 && charID <= 0x8CFC)
    {
        int tileDelta = charID - 0x8C40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1786, x, y);
        return;
    }
    //0x8D40 to 0x8DFC TileStart 1974
    else if (charID >= 0x8D40 && charID <= 0x8DFC)
    {
        int tileDelta = charID - 0x8D40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 1974, x, y);
        return;
    }
    //0x8E40 to 0x8EFC TileStart 2162
    else if (charID >= 0x8E40 && charID <= 0x8EFC)
    {
        int tileDelta = charID - 0x8E40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 2162, x, y);
        return;
    }
    //0x8F40 to 0x8FFC TileStart 2350
    else if (charID >= 0x8F40 && charID <= 0x8FFC)
    {
        int tileDelta = charID - 0x8F40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 2350, x, y);
        return;
    }
    //0x9040 to 0x90FC TileStart 2538
    else if (charID >= 0x9040 && charID <= 0x90FC)
    {
        int tileDelta = charID - 0x9040;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 2538, x, y);
        return;
    }
    //0x9140 to 0x91FC TileStart 2726
    else if (charID >= 0x9140 && charID <= 0x91FC)
    {
        int tileDelta = charID - 0x9140;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 2726, x, y);
        return;
    }
    //0x9240 to 0x92FC TileStart 2914
    else if (charID >= 0x9240 && charID <= 0x92FC)
    {
        int tileDelta = charID - 0x9240;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 2914, x, y);
        return;
    }
    //0x9340 to 0x93FC TileStart 3102
    else if (charID >= 0x9340 && charID <= 0x93FC)
    {
        int tileDelta = charID - 0x9340;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 3102, x, y);
        return;
    }
    //0x9440 to 0x94FC TileStart 3290
    else if (charID >= 0x9440 && charID <= 0x94FC)
    {
        int tileDelta = charID - 0x9440;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 3290, x, y);
        return;
    }
    //0x9540 to 0x95FC TileStart 3478
    else if (charID >= 0x9540 && charID <= 0x95FC)
    {
        int tileDelta = charID - 0x9540;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 3478, x, y);
        return;
    }
    //0x9640 to 0x9672 TileStart 3666
    else if (charID >= 0x9640 && charID <= 0x9672)
    {
        int tileDelta = charID - 0x9640;
        TileIDtoXY(tileDelta + 3666, x, y);
        return;
    }
    //0x969F to 0x96FC TileStart 3728
    else if (charID >= 0x969F && charID <= 0x96FC)
    {
        int tileDelta = charID - 0x969F;
        TileIDtoXY(tileDelta + 3728, x, y);
        return;
    }
    //0x9740 to 0x97FC TileStart 3822
    else if (charID >= 0x9740 && charID <= 0x97FC)
    {
        int tileDelta = charID - 0x9740;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 3822, x, y);
        return;
    }
    //0x9840 to 0x98FC TileStart 4010
    else if (charID >= 0x9840 && charID <= 0x98FC)
    {
        int tileDelta = charID - 0x9840;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4010, x, y);
        return;
    }
    //0x9940 to 0x99FC TileStart 4198
    else if (charID >= 0x9940 && charID <= 0x99FC)
    {
        int tileDelta = charID - 0x9940;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4198, x, y);
        return;
    }
    //0x9A40 to 0x9AFC TileStart 4386
    else if (charID >= 0x9A40 && charID <= 0x9AFC)
    {
        int tileDelta = charID - 0x9A40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4386, x, y);
        return;
    }
    //0x9B40 to 0x9BFC TileStart 4574
    else if (charID >= 0x9B40 && charID <= 0x9BFC)
    {
        int tileDelta = charID - 0x9B40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4574, x, y);
        return;
    }
    //0x9C40 to 0x9CFC TileStart 4762
    else if (charID >= 0x9C40 && charID <= 0x9CFC)
    {
        int tileDelta = charID - 0x9C40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4762, x, y);
        return;
    }
    //0x9D40 to 0x9DFC TileStart 4950
    else if (charID >= 0x9D40 && charID <= 0x9DFC)
    {
        int tileDelta = charID - 0x9D40;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 4950, x, y);
        return;
    }
    //0xE040 to 0xE0FC TileStart 5138
    else if (charID >= 0xE040 && charID <= 0xE0FC)
    {
        int tileDelta = charID - 0xE040;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 5138, x, y);
        return;
    }
    //0xE140 to 0xE1FC TileStart 5326
    else if (charID >= 0xE140 && charID <= 0xE1FC)
    {
        int tileDelta = charID - 0xE140;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 5326, x, y);
        return;
    }
    //0xE240 to 0xE2FC TileStart 5514
    else if (charID >= 0xE240 && charID <= 0xE2FC)
    {
        int tileDelta = charID - 0xE240;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 5514, x, y);
        return;
    }
    //0xE340 to 0xE3FC TileStart 5702
    else if (charID >= 0xE340 && charID <= 0xE3FC)
    {
        int tileDelta = charID - 0xE340;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 5702, x, y);
        return;
    }
    //0xE440 to 0xE4FC TileStart 5890
    else if (charID >= 0xE440 && charID <= 0xE4FC)
    {
        int tileDelta = charID - 0xE440;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 5890, x, y);
        return;
    }
    //0xE540 to 0xE5FC TileStart 6078
    else if (charID >= 0xE540 && charID <= 0xE5FC)
    {
        int tileDelta = charID - 0xE540;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 6078, x, y);
        return;
    }
    //0xE640 to 0xE6FC TileStart 6266
    else if (charID >= 0xE640 && charID <= 0xE6FC)
    {
        int tileDelta = charID - 0xE640;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 6266, x, y);
        return;
    }
    //0xE740 to 0xE7FC TileStart 6454
    else if (charID >= 0xE740 && charID <= 0xE7FC)
    {
        int tileDelta = charID - 0xE740;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 6454, x, y);
        return;
    }
    //0xE840 to 0xE8FC TileStart 6642
    else if (charID >= 0xE840 && charID <= 0xE8FC)
    {
        int tileDelta = charID - 0xE840;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 6642, x, y);
        return;
    }
    //0xE940 to 0xE9FC TileStart 6830
    else if (charID >= 0xE940 && charID <= 0xE9FC)
    {
        int tileDelta = charID - 0xE940;
        if((charID & 0xFF) == 0x7f){
            TileIDtoXY(96, x, y);
            return;
        }
        if((charID & 0xFF) > 0x7f) tileDelta -= 1;
        TileIDtoXY(tileDelta + 6830, x, y);
        return;
    }
    //0xEA40 to 0xEA7E TileStart 7018
    else if (charID >= 0xEA40 && charID <= 0xEA7E)
    {
        int tileDelta = charID - 0xEA40;
        TileIDtoXY(tileDelta + 7018, x, y);
        return;
    }
    //0xEA80 to 0xEAA4 TileStart 7081
    else if (charID >= 0xEA80 && charID <= 0xEAA4)
    {
        int tileDelta = charID - 0xEA80;
        TileIDtoXY(tileDelta + 7081, x, y);
        return;
    }
    //0xED40 to 0xED6E TileStart 7136
    else if (charID >= 0xED40 && charID <= 0xED6E)
    {
        int tileDelta = charID - 0xED40;
        TileIDtoXY(tileDelta + 7136, x, y);
        return;
    }
    //0xFA40 to 0xFA57 TileStart 7120
    else if (charID >= 0xFA40 && charID <= 0xFA57)
    {
        int tileDelta = charID - 0xFA40;
        TileIDtoXY(tileDelta + 7120, x, y);
        return;
    }
    else
    {
        *x = 0;
        *y = 96;
    }
}

void GetCharDetails(int charID, int * x, int * y, int * width, int * height, int * srcImgWidth, u8 ** src)
{
    //Check if it's ASCII
    if((charID < 0x80) || (charID >= 0xa0 && charID < 0xe0))
    {
        GetFontCharXYW(charID, x, y, width);
        *height = 16;
        *src = (u8*)&HWFontData;
        *srcImgWidth = HWFontWidth;
    }
    else
    {
        //Else it's probably SJIS. Who cares
        GetSJISCharXY(charID, x, y);
        *height = 16;
        *width = 14;
        *src = (u8*)FWFontData;
        *srcImgWidth = FWFontWidth;
    }
}

void BlitString(int row, unsigned char *text)
{
    int dstX = 0;

    int i = 0;
    int d = 0;
    int stln = ccKanjiStrLen(text);

    int srcX = 0;
    int srcY = 0;
    int width = 0;
    int height = 0;
    u8 * src = NULL;
    int srcImgWidth = 0;

    for(i = 0; i < stln; i++)
    {
        u8 c = text[d];
        //Check out ASCII first...
        if(c < 0x80)
        {
            GetCharDetails(c, &srcX, &srcY, &width, &height, &srcImgWidth, &src);
            srcImgWidth = HWFontWidth;
            src = (u8*)HWFontData;
            d++;
        }
        else if((c >= 0xa0) && (c  < 0xe0))
        {
            GetCharDetails(c, &srcX, &srcY, &width, &height, &srcImgWidth, &src);
            srcImgWidth = HWFontWidth;
            src = (u8*)HWFontData;
            d++;
        }
        else
        {
            //printf("GETTING CHAR DETAILS %04X\n", (c << 8) + text[d + 1]);
            GetCharDetails((c << 8) | text[d + 1], &srcX, &srcY, &width, &height, &srcImgWidth, &src);
            srcImgWidth = FWFontWidth;
            src = (u8*)FWFontData;
            d += 2;
        }

        if(dstX + width >= 256) break;
        if(BlitFontData == NULL) printf("Cannot BlitText, the output buffer is NULL\n");
        CopyRect(srcX, srcY, dstX, row * 16, width, height, src, BlitFontData, srcImgWidth, 256);
        dstX += width;
    }
    BlitLineWidths[row] = dstX;
    UpdateBlitTex();
}

void UpdateBlitTex()
{
    //gsKit_setup_tbw(&gsBlitTex);

    gsKit_texture_send(gsBlitFont.Mem, 256, 256, gsBlitFont.Vram, GS_PSM_T4, gsBlitFont.TBW, GS_CLUT_TEXTURE);
    //printf("UPDATED BLITTEX AT: 0x%08X\n", gsBlitFont.Mem);
}

void DrawBlitTex(int row, int x, int y)
{
    DrawBlitTexColor(row, x, y, 0x80808080);
}

void DrawBlitTexColor(int row, int x, int y, u32 color)
{
    int width = BlitLineWidths[row];
    gsKit_prim_sprite_texture_3d(gsGlobal, &gsBlitFont, x, y, 0.0f, 0.0f, row * 16, x +  width, y + 16, 0.0f, width, (row + 1) * 16, color);

}
