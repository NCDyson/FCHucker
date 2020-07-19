#ifndef BLITTEXT_H
#define BLITTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>
#include "util.h"


#define ASCII_LOWER_Y       0
#define ASCII_UPPER_Y       96

extern int BlitLineWidths[];

void CopyRect(int srcX, int srcY, int dstX, int dstY, int width, int height, u8 * src, u8 * dst, int srcImgWidth, int dstImgWidth);

void BlitString(int y, unsigned char *text);

void UpdateBlitTex();

void DrawBlitTex(int row, int x, int y);
void DrawBlitTexColor(int row, int x, int y, u32 color);
#endif // BLITTEXT_H
