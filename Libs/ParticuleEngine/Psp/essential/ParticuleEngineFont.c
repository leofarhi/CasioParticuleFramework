#include "ParticuleEngineOverwrited.h"

PC_Font* MainFont = NULL;

PC_Font* PC_LoadFont(const char* path, int size)
{
    FILE* file = __psp_openBin(path);
    if (file == NULL)
        PC_ErrorScreen("Error: File not found");
    PC_Texture* texture = _PC_LoadTexture(file);
    int w, h, row, col;
    fread(&w, sizeof(int), 1, file);
    fread(&h, sizeof(int), 1, file);
    fread(&row, sizeof(int), 1, file);
    fread(&col, sizeof(int), 1, file);
    fclose(file);
    PC_Font* font = malloc(sizeof(PC_Font));
    font->texture = texture;
    font->w = w;
    font->h = h;
    font->row = row;
    font->col = col;
    font->size = size;
    char str[100];
    return font;
}

void PC_FreeFont(PC_Font* font)
{
    if (font == NULL)
        return;
    if (font->texture != NULL)
        PC_FreeTexture(font->texture);
    free(font);
}

void PC_DrawText(const unsigned char *text, int x, int y, PC_Color color, PC_Font* font)
{
    if (font == NULL)
        return;
    int t = fmax(font->h, font->w);
    int scale = font->size / t / 2;
    int len = strlen((const char *)text);
    for(int i = 0; i < len; i++){
        unsigned char c = text[i];
        int row = c / font->col;
        int col = c % font->col;
        int u = col * font->w;
        int v = row * font->h;
        PC_DrawSubTextureSizeColored(font->texture, x + i * font->w * scale, y, u, v, font->w, font->h, font->w * scale, font->h * scale, color);
    }
}

void PC_SetMainFont(PC_Font* font)
{
    MainFont = font;
}

Vector2 PC_GetTextSize(const unsigned char *text, PC_Font* font)
{
    if (font == NULL)
        return (Vector2){0,0};
    int t = fmax(font->h, font->w);
    int scale = font->size / t;
    int len = strlen((const char *)text);
    return (Vector2){len * font->w * scale, font->h * scale};
}