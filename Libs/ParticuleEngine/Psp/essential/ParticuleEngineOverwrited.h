#ifndef PARTICULE_ENGINE_OVERWRITED_H
#define PARTICULE_ENGINE_OVERWRITED_H

#define OVERWRITE_SCREEN_SIZE
#define OVERWRITE_STRUCT_FONT
#define OVERWRITE_STRUCT_TEXTURE
#define OVERWRITE_STRUCT_COLOR

#include "../../../All/include/essential/ParticuleEngineEssential.h"

#include <common/common-sce.h>
#include <common/callbacks.h>
#include <common/image.h>
#include <MyKeyboard/MyKeyboard.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <psputility.h>

#ifndef PROJECT_NAME
#define PROJECT_NAME "Engine"
#endif

//Screen
#define PSP_BUF_WIDTH (512)
#define PSP_SCR_WIDTH (480)
#define PSP_SCR_HEIGHT (272)


unsigned int __attribute__((aligned(16))) PSP_list[262144];

#define SCREEN_WIDTH PSP_SCR_WIDTH // 480
#define SCREEN_HEIGHT PSP_SCR_HEIGHT // 272

struct __pc_VramDisplay
{
    u16* vram16;
    u32* vram32;
    int bufferwidth;
    int pixelformat;
    int unknown;
    void(*PutPixel)(int x, int y, u32 color);
};

extern struct __pc_VramDisplay __pc_vram;

//Input
extern struct SceCtrlData __pc_padData;
extern struct SceCtrlData __pc_oldData;

float InputAnalogX();
float InputAnalogY();

//Draw
typedef struct PC_Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    u32 color;
} PC_Color;

//Texture
typedef struct PC_Texture
{
    image_t* texture;
} PC_Texture;
FILE* __psp_openBin(const char* path);
void _PC_DrawVramPixel(int x, int y, u32 color);
PC_Texture* _PC_LoadTexture(FILE* file);

//Font
typedef struct PC_Font
{
    PC_Texture* texture;
    int w, h;
    int row, col;
    int size;
} PC_Font;



#include "../../../All/include/essential/ParticuleEngineGraphic.h"

#endif