#ifndef PARTICULE_ENGINE_OVERWRITED_H
#define PARTICULE_ENGINE_OVERWRITED_H

#define OVERWRITE_SCREEN_SIZE
#define OVERWRITE_STRUCT_FONT
#define OVERWRITE_STRUCT_TEXTURE
#define OVERWRITE_STRUCT_COLOR

#include "../../../All/include/essential/ParticuleEngineEssential.h"

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/display.h>
#include <gint/rtc.h>
#include <gint/gint.h>
#include <gint/timer.h>
#include <gint/clock.h>
#include <MyKeyboard.h>
#include <stdarg.h>
#include <gint/clock.h>
#include <time.h>
//#include <libprof.h>

//Screen
#define SCREEN_WIDTH 396
#define SCREEN_HEIGHT 224

//Input
typedef struct 
{
    int key;
    int type;
} PC_GINT_keyinfo;

extern List* InputEvents;

//Font
typedef struct PC_Font
{
    font_t* font;
    bool isAllocated;
} PC_Font;

//Texture
typedef struct PC_Texture PC_Texture;
typedef struct PC_Texture
{
    bopti_image_t* texture;
    char* IsExternalPath;
    int (*GetPixel)(PC_Texture* texture, int x, int y);
    int (*DecodePixel)(PC_Texture* texture, int pixel);
    void (*SetPixel)(PC_Texture* texture, int x, int y, int color);
    int _alphaValue;
    bool isAllocated;
} PC_Texture;

typedef struct PC_Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    int color;
} PC_Color;

#include "../../../All/include/essential/ParticuleEngineGraphic.h"

#endif