#ifndef FONTS_H
#define FONTS_H
#define INCBIN_PREFIX
#include "incbin.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

#include "ui.h"

extern unsigned int FWFontWidth;
extern unsigned int FWFontHeight;
INCBIN_EXTERN(FWFont);

extern unsigned int HWFontWidth;
extern unsigned int HWFontHeight;
INCBIN_EXTERN(HWFont);

extern unsigned int FontCLUTWidth;
extern unsigned int FontCLUTHeight;
INCBIN_EXTERN(FontCLUT);

GSTEXTURE gsHWFont;
GSTEXTURE gsBlitFont;
u8 * BlitFontData;

void InitFonts();
void GetFontCharXYW(int id, int * x, int * y, int * w);
int GetFontCharWidth(int id);

/**
 * @brief GetStringWidth
 * @param text
 * @return Draw width of string
 * Ignores Newline, so width is based on entire string as one line.
 */
int GetStringWidth(unsigned char * text);
void DrawString(int x, int y, unsigned char * text);
void DrawStringCentered(int x, int y, unsigned char * text);
int DrawCharacterWithBG(int x, int y, unsigned char id, u64 BGColor);


#endif // FONTS_H
