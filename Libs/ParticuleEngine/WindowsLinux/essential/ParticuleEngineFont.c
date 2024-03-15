#include "ParticuleEngineOverwrited.h"

PC_Font* MainFont = NULL;

PC_Font* PC_LoadFont(const char* path, int size)
{
    PC_Font* font = malloc(sizeof(PC_Font));
    font->font = TTF_OpenFont(path, size);
    if (font->font == NULL)
    {
        printf("Error loading font: %s\n", TTF_GetError());
        exit(1);
    }
    return font;
}

void PC_FreeFont(PC_Font* font)
{
    if (font == NULL)
        return;
    TTF_CloseFont(font->font);
    free(font);
}

void PC_DrawText(const unsigned char *text, int x, int y, PC_Color color, PC_Font* font)
{
    if (font == NULL)
        return;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    //SDL_Surface* textSurface = TTF_RenderText_Blended(font->font, text, sdlColor);
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font->font, (const char *)text, sdlColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(__sdl_renderer, textSurface);
    SDL_Rect rect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(__sdl_renderer, textTexture, NULL, &rect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void PC_SetMainFont(PC_Font* font)
{
    MainFont = font;
}

Vector2 PC_GetTextSize(const unsigned char *text, PC_Font* font)
{
    if (font == NULL)
        return (Vector2){0,0};
    SDL_Color sdlColor = {255, 255, 255, 255};
    //SDL_Surface* textSurface = TTF_RenderText_Blended(font->font, text, sdlColor);
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font->font, (const char*) text, sdlColor);
    Vector2 size = {textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
    return size;
}