#include "ParticuleEngineOverwrited.h"

PC_Color PC_ColorCreate(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    PC_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

bool PC_ColorEqual(PC_Color color1, PC_Color color2)
{
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}

void PC_DrawPoint(int x, int y, PC_Color color)
{
    SDL_SetRenderDrawColor(__sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(__sdl_renderer, x, y);
}

void PC_DrawLine(int x1, int y1, int x2, int y2, PC_Color color)
{
    SDL_SetRenderDrawColor(__sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(__sdl_renderer, x1, y1, x2, y2);
}

void PC_DrawRect(int x, int y, int w, int h, PC_Color color)
{
    SDL_SetRenderDrawColor(__sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(__sdl_renderer, &rect);
}

void PC_DrawFilledRect(int x, int y, int w, int h, PC_Color color)
{
    SDL_SetRenderDrawColor(__sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(__sdl_renderer, &rect);
}