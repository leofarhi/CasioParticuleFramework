#ifndef PARTICULE_ENGINE_FONT_H
#define PARTICULE_ENGINE_FONT_H
#include "ParticuleEngineGraphic.h"

#ifndef OVERWRITE_STRUCT_FONT
typedef struct PC_Font
{
    void* overwrited;    
} PC_Font;
#endif

extern PC_Font* MainFont;

PC_Font* PC_LoadFont(const char* path, int size);

void PC_FreeFont(PC_Font* font);

void PC_DrawText(const unsigned char *text, int x, int y, PC_Color color, PC_Font* font);

void PC_SetMainFont(PC_Font* font);

Vector2 PC_GetTextSize(const unsigned char *text, PC_Font* font); //tres couteux

#endif