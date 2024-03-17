#ifndef PARTICULE_ENGINE_DRAW_H
#define PARTICULE_ENGINE_DRAW_H
#include "ParticuleEngineGraphic.h"
#include "ParticuleEngineEssential.h"

#ifndef OVERWRITE_STRUCT_COLOR
typedef struct PC_Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} PC_Color;
#endif

PC_Color PC_ColorCreate(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

bool PC_ColorEqual(PC_Color color1, PC_Color color2);
//returns true if the two colors are equal, false otherwise

void PC_DrawPixel(int x, int y, PC_Color color);
//Draw a point on the screen at (x, y) with the given color

void PC_DrawLine(int x1, int y1, int x2, int y2, PC_Color color);
//Draw a line on the screen from (x1, y1) to (x2, y2) with the given color

void PC_DrawRect(int x, int y, int w, int h, PC_Color color);
//Draw a rectangle on the screen at (x, y) with the given width and height and color

void PC_DrawFilledRect(int x, int y, int w, int h, PC_Color color);
//Draw a filled rectangle on the screen at (x, y) with the given width and height and color


#define PC_WHITE PC_ColorCreate(255,255,255,255)
#define PC_BLACK PC_ColorCreate(0,0,0,255)

#endif