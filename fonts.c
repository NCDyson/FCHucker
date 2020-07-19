#define INCBIN_PREFIX
#include "incbin.h"
#include "fonts.h"
#include "graphics.h"
#include "UITileset.h"

unsigned int FWFontWidth = 224;
unsigned int FWFontHeight = 7184;
INCBIN(FWFont, "FontFull.bin");

unsigned int HWFontWidth = 128;
unsigned int HWFontHeight = 256;
INCBIN(HWFont, "FontHalf.bin");

unsigned int FontCLUTWidth = 16;
unsigned int FontCLUTHeight = 16;
INCBIN(FontCLUT, "FontCLUT.bin");

const unsigned char FontTable[] = {
    0x7,
    0x5,
    0x5,
    0x8,
    0x7,
    0x8,
    0x7,
    0x5,
    0x6,
    0x5,
    0x8,
    0x7,
    0x6,
    0x7,
    0x7,
    0x7,
    0x7,
    0x6,
    0x8,
    0x8,
    0x8,
    0x8,
    0x7,
    0x7,
    0x8,
    0x8,
    0x4,
    0x4,
    0x6,
    0x8,
    0x6,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x3,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x6,
    0x7,
    0x7,
    0x7,
    0x7,
    0x6,
    0x7,
    0x5,
    0x7,
    0x5,
    0x6,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x3,
    0x8,
    0x8,
    0x4,
    0x8,
    0x7,
    0x7,
    0x7,
    0x7,
    0x6,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x7,
    0x3,
    0x7,
    0x7,
    0x0,
};

void InitFonts()
{
    gsHWFont.Width = HWFontWidth;
    gsHWFont.Height = HWFontHeight;
    gsHWFont.PSM = GS_PSM_T4;
    gsHWFont.Filter = GS_FILTER_NEAREST;
    gsHWFont.ClutPSM = GS_PSM_CT32;

    int textureSize = gsKit_texture_size_ee(gsHWFont.Width, gsHWFont.Height, gsHWFont.PSM);
    gsHWFont.Mem = memalign(128, textureSize);
    memcpy(gsHWFont.Mem, HWFontData, textureSize);

    gsHWFont.Clut = memalign(128, gsKit_texture_size_ee(4, 4, GS_PSM_CT32));
    memcpy(gsHWFont.Clut, FontCLUTData, FontCLUTSize);

    gsHWFont.Vram = gsKit_vram_alloc(gsGlobal, gsKit_texture_size(gsHWFont.Width, gsHWFont.Height, gsHWFont.PSM), GSKIT_ALLOC_USERBUFFER);
    gsHWFont.VramClut = gsKit_vram_alloc(gsGlobal, gsKit_texture_size(4, 4, GS_PSM_CT32), GSKIT_ALLOC_USERBUFFER);

    //gsKit_setup_tbw(&gsHWFont);
    //gsKit_texture_send(gsHWFont.Mem, gsHWFont.Width, gsHWFont.Height, gsHWFont.Vram, GS_PSM_T8, gsHWFont.TBW, GS_CLUT_TEXTURE);
    //gsKit_texture_send(gsHWFont.Clut, 16, 16, gsHWFont.VramClut, gsHWFont.ClutPSM, 1, GS_CLUT_PALLETE);
    gsKit_texture_upload(gsGlobal, &gsHWFont);

    gsBlitFont.Width = 256;
    gsBlitFont.Height = 256;
    gsBlitFont.PSM = GS_PSM_T4;
    gsBlitFont.Filter = GS_FILTER_NEAREST;

    textureSize = gsKit_texture_size_ee(256, 256, gsBlitFont.PSM);
    gsBlitFont.Mem = memalign(128, textureSize);
    memset(gsBlitFont.Mem, 0, (256 * 128));
    BlitFontData = (u8*)gsBlitFont.Mem;
    gsBlitFont.Clut = gsHWFont.Clut;
    gsBlitFont.VramClut = gsHWFont.VramClut;
    gsBlitFont.Vram = gsKit_vram_alloc(gsGlobal, gsKit_texture_size(256, 256, GS_PSM_T8), GSKIT_ALLOC_USERBUFFER);
    gsKit_setup_tbw(&gsBlitFont);

}

void GetFontCharXYW(int id, int *x, int *y, int *w)
{
    *x = 0;
    *y = 0;
    *w = 7;
    if(id >= 0x20 && id < 0x80)
    {
        id -= 0x20;
        *y = (id / 16) * 16;
        *x = (id % 16) * 8;
        *w = FontTable[id];
    }
    else if(id >= 0xa0 && id < 0xe0)
    {
        id -= 0x40;
        *y = (id / 16) * 16;
        *x = (id % 16) * 8;
        *w = 0x7;
    }
}

void UpdateBlitFont()
{
    gsKit_texture_send(gsBlitFont.Mem, gsBlitFont.Width, gsBlitFont.Height, gsBlitFont.Vram, gsBlitFont.PSM, gsBlitFont.TBW, GS_CLUT_TEXTURE);
}

int GetFontCharWidth(int id)
{
    if(id >= 0x20 && id < 0x80)
    {
        id -= 0x20;
        return FontTable[id];
    }
    else
    {
        return 0x7;
    }
}

/*Not for other translation units*/
int IsTileCode(u8 t)
{
    switch (t) {
    case 0x43: //Circle
        return TILE_Button_Circle;
    case 0x53: //Square
        return TILE_Button_Square;
    case 0x58: //CROSS
        return TILE_Button_Cross;
    case 0x54: //TRIANGLE
        return TILE_Button_Triangle;
    default:
        return 0;
    }
}

int GetStringWidth(unsigned char *text)
{
    int result = 0;
    unsigned long i = 0;
    unsigned long slen = strlen((const char *)text);
    for(i = 0; i < slen; i++)
    {
        u8 c = text[i];
        if((c >= 0x20 && c < 0x80) || (c > 0xa0))
        {
            int tile = 0;
            if(c == 0x5b)
            {
                //check for overflow
                if(i < (slen - 2))
                {
                    //check for closed
                    if((u8)text[i + 2] == 0x5d)
                    {
                        if(IsTileCode((u8)text[i + 1]))
                        {
                            tile = 16;
                            i += 2;
                        }
                    }
                }
            }
            result += tile ? tile : GetFontCharWidth(c);
        }
    }
    return result;
}

void DrawString(int x, int y, unsigned char *text)
{
    unsigned long i = 0;
    int tx = 0;
    int ty = 0;
    int tw = 0;
    int currentX = x;
    int currentY = y;
    unsigned long slen = strlen((const char *)text);
    for(i = 0; i < slen; i++)
    {
        int c = text[i];
        if(c == 0xa)
        {
            currentX = x;
            currentY += 20;
        }
        else if((c >= 0x20 && c < 0x80) || (c >= 0xa0 && c < 0xE0))
        {
            int tile = 0;
            if(c == 0x5b)
            {
                //check for overflow
                if(i < (slen - 2))
                {
                    //check for closed
                    if((u8)text[i + 2] == 0x5d)
                    {
                        tile = IsTileCode((u8)text[i+1]);
                    }
                }
            }

            if(tile)
            {
                i += 2;
                DrawTileID(currentX, currentY, tile);
                currentX += 16;
            }
            else
            {
                GetFontCharXYW(c, &tx, &ty, &tw);
                gsKit_prim_sprite_texture_3d(gsGlobal, &gsHWFont, currentX, currentY, 0.0f, tx, ty, currentX + 8, currentY + 16, 0.0f, tx + 8, ty + 16, 0x80808080);
                currentX += tw;
            }
        }

    }
}

int DrawCharacterWithBG(int x, int y, unsigned char id, u64 BGColor)
{
    if((id >= 0x20 && id < 0x80) || (id > 0xa0 && id < 0xE0))
    {
        int cx = 0;
        int cy = 0;
        int cw = 0;
        GetFontCharXYW(id, &cx, &cy, &cw);
        gsKit_prim_sprite(gsGlobal, x, y, x + cw, y + 16, 0.0f, BGColor);
        gsKit_prim_sprite_texture_3d(gsGlobal, &gsHWFont, x, y, 0.0f, cx, cy, x + cw, y + 16, 0.0f, cx + 8, cy + 16, 0x80808080);
        return cw;
    }
    return 0;
}

void DrawStringCentered(int x, int y, unsigned char *text)
{
    int strwidth = GetStringWidth(text);
    DrawString(x - (strwidth / 2), y, text);
}
