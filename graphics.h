#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <gsToolkit.h>

#include "UITex.h"


GSGLOBAL * gsGlobal;
GSTEXTURE gsUITex;


u64 ClearColor;
u64 TexColor;
extern int gsKit_texture_finish(GSGLOBAL *gsGlobal, GSTEXTURE *Texture);
void GraphicsInit();

int gsKit_texture_rawdata(GSGLOBAL *gs, GSTEXTURE *Texture, unsigned char * data);

void Draw();
#endif // GRAPHICS_H
