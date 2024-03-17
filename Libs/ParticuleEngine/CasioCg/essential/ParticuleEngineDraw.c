#include "ParticuleEngineOverwrited.h"

PC_Color PC_ColorCreate(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    PC_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    color.color = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
    return color;
}

bool PC_ColorEqual(PC_Color color1, PC_Color color2)
{
    if (color1.a == 0 && color2.a == 0)
        return true;
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}

void PC_DrawPixel(int x, int y, PC_Color color)
{
    dpixel(x, y, color.color);
}

void PC_DrawLine(int x1, int y1, int x2, int y2, PC_Color color)
{
    dline(x1, y1, x2, y2, color.color);
}

void PC_DrawRect(int x, int y, int w, int h, PC_Color color)
{
    if (w <= 0 || h <= 0)
        return;
    w--;
    h--;
    dline(x, y, x + w, y, color.color);
    dline(x + w, y, x + w, y + h, color.color);
    dline(x, y + h, x, y, color.color);
    dline(x, y + h, x + w, y + h, color.color);
}

void PC_DrawFilledRect(int x, int y, int w, int h, PC_Color color)
{
    if (w <= 0 || h <= 0)
        return;
    w--;
    h--;
    drect(x, y, x + w, y + h, color.color);
}