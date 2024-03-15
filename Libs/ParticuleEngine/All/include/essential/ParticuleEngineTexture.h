#ifndef PARTICULE_ENGINE_TEXTURE_H
#define PARTICULE_ENGINE_TEXTURE_H
#include "ParticuleEngineGraphic.h"

#ifndef OVERWRITE_STRUCT_TEXTURE
typedef struct PC_Texture
{
    void* overwrited;
} PC_Texture;
#endif

int GetWidth(PC_Texture* texture);
//returns the width of a texture
//texture is a pointer to a PC_Texture struct

int GetHeight(PC_Texture* texture);
//returns the height of a texture
//texture is a pointer to a PC_Texture struct

PC_Texture* PC_LoadTexture(const char* path);
//Loads a texture from a file
//path is a path to a file
//returns a pointer to a PC_Texture struct

PC_Texture* PC_LoadSubTextureSize(const char* path, int sx, int sy, int sw, int sh);
//Loads a part of a texture from a file
//path is a path to a file
//sx and sy are the coordinates of the top left corner of the part of the texture to load
//sw and sh are the width and height of the part of the texture to load


void PC_FreeTexture(PC_Texture* texture);
//frees the memory allocated for the texture
//texture is a pointer to a PC_Texture struct

void PC_DrawTexture(PC_Texture* texture, int x, int y);
//draws a texture on the screen
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the top left corner of the texture

void PC_DrawSubTexture(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh);
//draws a part of a texture on the screen
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the top left corner of the texture
//sx and sy are the coordinates of the top left corner of the part of the texture to draw
//sw and sh are the width and height of the part of the texture to draw

void PC_DrawSubTextureSize(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h);
//draws a part of a texture on the screen
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the top left corner of the texture
//sx and sy are the coordinates of the top left corner of the part of the texture to draw
//sw and sh are the width and height of the part of the texture to draw
//w and h are the width and height of the texture to draw

void PC_DrawSubTextureSizeColored(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h, PC_Color color);
//draws a part of a texture on the screen
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the top left corner of the texture
//sx and sy are the coordinates of the top left corner of the part of the texture to draw
//sw and sh are the width and height of the part of the texture to draw
//w and h are the width and height of the texture to draw
//color is the color of the texture

void PC_DrawSubTextureRect(PC_Texture* texture, PC_Rect* src, PC_Rect* dest, PC_Color TextureColorMod);
//draws a part of a texture on the screen
//texture is a pointer to a PC_Texture struct
//src is a pointer to a PC_Rect struct containing the coordinates of the top left corner of the part of the texture to draw and the width and height of the part of the texture to draw
//dest is a pointer to a PC_Rect struct containing the coordinates of the top left corner of the texture and the width and height of the texture to draw


PC_Color PC_GetPixelColor(PC_Texture* texture, int x, int y);
//returns the color of a pixel in a texture
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the pixel

void PC_SetPixelColor(PC_Texture* texture, int x, int y, PC_Color color);
//##### WORK ONLY WITH ALLOCATED TEXTURE #####
//sets the color of a pixel in a texture
//texture is a pointer to a PC_Texture struct
//x and y are the coordinates of the pixel
//color is the color to set

PC_Texture* PC_LoadTextureSprite(const char* path,int index);
//Loads a texture from a file among a sprite sheet
//path is a path to a file
//index is the index of the texture in the sprite sheet

#endif