#include "ParticuleEngineOverwrited.h"

PC_Font* MainFont = NULL;

PC_Font* PC_LoadFont(const char* path, int size)
{
    (void)size;
    VirtualFile* file = PC_LoadResource((unsigned char*)path);
    if (file == NULL)
    {
        PC_Font* font = (PC_Font*)malloc(sizeof(PC_Font));
        font->font = NULL;
        font->isAllocated = true;
        return font;
    }
    else
    {
        PC_Font* font = (PC_Font*)file->data;
        font->isAllocated = false;
        return font;
    }
}

void PC_FreeFont(PC_Font* font)
{
    if (font->isAllocated)
        free(font);
}

void PC_DrawText(const unsigned char *text, int x, int y, PC_Color color, PC_Font* font)
{
    if (font != NULL)
        dfont(font->font);
    else
        dfont(dfont_default());
    dtext(x, y, color.color, (const char*)text);
}

void PC_SetMainFont(PC_Font* font)
{
    MainFont = font;
    dfont(font->font);
}

Vector2 PC_GetTextSize(const unsigned char *text, PC_Font* font)
{
    Vector2 size = {0,0};
    if (font == NULL)
        dsize((char const *)text,dfont_default(), &size.x, &size.y);
    else
        dsize((char const *)text,font->font, &size.x, &size.y);
    return size;
}