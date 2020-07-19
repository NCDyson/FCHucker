#include "graphics.h"
#include "windowstack.h"
GSGLOBAL * gsGlobal = NULL;


void GraphicsInit()
{
    gsGlobal = gsKit_init_global();

    ClearColor = GS_SETREG_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x00);
    TexColor = GS_SETREG_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x00);

    gsGlobal->PSM = GS_PSM_CT32;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    printf("DISPLAY MODE: %d, %d\n", gsGlobal->Width, gsGlobal->Height);
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    gsGlobal->DoubleBuffering = GS_SETTING_ON;
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);

    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    //gsKit_set_primalpha(gsGlobal, GS_SETREG_ALPHA(0, 1, 0, 1, 0), 0);
    gsKit_set_primalpha(gsGlobal, GS_SETREG_ALPHA(0, 1, 0, 1, 0 ), 0);
    gsKit_set_test(gsGlobal, GS_ATEST_ON);
    gsKit_set_test(gsGlobal, GS_ZTEST_ON);

    //gsKit_mode_switch(gsGlobal, GS_PERSISTENT);

    gsKit_clear(gsGlobal, ClearColor);

    gsUITex.Width = UITexWidth;
    gsUITex.Height = UITexHeight;
    gsUITex.PSM = GS_PSM_CT32;
    gsUITex.Filter = GS_FILTER_NEAREST;
    gsKit_texture_rawdata(gsGlobal, &gsUITex, (unsigned char *)UITexData);
    //printf("TEXTURE VRAM: %08X\n", gsUITex.Vram);

    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
}

int gsKit_texture_rawdata(GSGLOBAL *gs, GSTEXTURE *Texture, unsigned char *data)
{
    int textureSize = gsKit_texture_size_ee(Texture->Width, Texture->Height, Texture->PSM);
    //printf("TEXTURE SIZE: %08X\n", textureSize);
    Texture->Mem = memalign(128, textureSize);
    if(Texture->PSM != GS_PSM_T8 && Texture->PSM != GS_PSM_T4)
    {
        Texture->VramClut = 0;
        Texture->Clut = 0;
    }
    //printf("TEXTURE: %08X\n", Texture->Mem);

    //TODO: check this or something...
    memcpy(Texture->Mem, data, textureSize);

    return gsKit_texture_finish(gs, Texture);
}

void Draw()
{
    gsGlobal->PrimAlphaEnable = GS_SETTING_OFF;
    gsKit_clear(gsGlobal, ClearColor);

    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    DrawWindows(gsGlobal);

    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);

}
