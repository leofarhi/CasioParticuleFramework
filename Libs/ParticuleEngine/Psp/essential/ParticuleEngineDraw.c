#include "ParticuleEngineOverwrited.h"

PC_Color PC_ColorCreate(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    PC_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    color.color = (a << 24) | r | (g << 8) | (b << 16);
    return color;
}

bool PC_ColorEqual(PC_Color color1, PC_Color color2)
{
    if (color1.a == 0 && color2.a == 0)
        return true;
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}

void _PC_DrawVramPixel(int x, int y, u32 color)
{
    if (__pc_vram.pixelformat == PSP_DISPLAY_PIXEL_FORMAT_8888)
        __pc_vram.vram32[x + y * __pc_vram.bufferwidth] = color;
}

void PC_DrawPixel(int x, int y, PC_Color color)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return;
    //__pc_vram.PutPixel(x, y, color.color);
    if (__pc_vram.pixelformat == PSP_DISPLAY_PIXEL_FORMAT_8888)
        __pc_vram.vram32[x + y * __pc_vram.bufferwidth] = color.color;
}

void PC_DrawLine(int x1, int y1, int x2, int y2, PC_Color color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;
    while (x1 != x2 || y1 != y2)
    {
        PC_DrawPixel(x1, y1, color);
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
    PC_DrawPixel(x2, y2, color);
}

void PC_DrawRect(int x, int y, int w, int h, PC_Color color)
{
    if (w <= 0 || h <= 0)
        return;
    w--;
    h--;
    PC_DrawLine(x, y, x + w, y, color);
    PC_DrawLine(x + w, y, x + w, y + h, color);
    PC_DrawLine(x + w, y + h, x, y + h, color);
    PC_DrawLine(x, y + h, x, y, color);
}

void PC_DrawFilledRect(int x, int y, int w, int h, PC_Color color)
{
    if (w <= 0 || h <= 0)
        return;
    for (int i = 0; i < h; i++)
        PC_DrawLine(x, y + i, x + w, y + i, color);
}