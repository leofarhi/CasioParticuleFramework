#include "ParticuleEngineOverwrited.h"

PC_Color __psp_imgC_ToRGB(int color){
    int r = ((color >> 11) & 0b11111) << 3;
    int g = ((color >> 5) & 0b111111) << 2;
    int b = (color & 0b11111) << 3;
    int a = 255;
    return PC_ColorCreate(r,g,b,a);
}

int GetWidth(PC_Texture* texture)
{
    return texture->texture->width;
}

int GetHeight(PC_Texture* texture)
{
    return texture->texture->height;
}

FILE* __psp_openBin(const char* path)
{
    char* path2 = malloc(strlen(path) + 10);
    sprintf(path2, "%s.bin", path);
    FILE* file = stbi__fopen(path2, "rb");
    free(path2);
    return file;
}

PC_Texture* _PC_LoadTexture(FILE* file)
{
    image_t* image = image_load(file);
    if (image == NULL)
        PC_ErrorScreen("Error: can't load texture");
    PC_Texture* texture = malloc(sizeof(PC_Texture));
    texture->texture = image;
    return texture;
}

PC_Texture* PC_LoadTexture(const char* path)
{
    FILE* file = __psp_openBin(path);
    if (file == NULL)
        PC_ErrorScreen("Error: File not found");
    PC_Texture* texture = _PC_LoadTexture(file);
    fclose(file);
    return texture;
}

PC_Texture* PC_CreateTexture(int width, int height)
{
    PC_Texture* texture = malloc(sizeof(PC_Texture));
    texture->texture = image_alloc(width, height,IMAGE_RGB565A);
    return texture;
}

void PC_FreeTexture(PC_Texture* texture)
{
    if (texture == NULL)
        return;
    if (texture->texture != NULL)
        image_free(texture->texture);
    texture->texture = NULL;
    free(texture);
}

void PC_DrawTexture(PC_Texture* texture, int x, int y)
{
    PC_DrawSubTexture(texture, x, y, 0, 0, GetWidth(texture), GetHeight(texture));
}

void PC_DrawSubTexture(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh)
{
    if(x < 0) {
        sx -= x;
        sw += x;
        x = 0;
    }
    if(y < 0) {
        sy -= y;
        sh += y;
        y = 0;
    }
    if(x + sw > SCREEN_WIDTH) sw = SCREEN_WIDTH - x;
    if(y + sh > SCREEN_HEIGHT) sh = SCREEN_HEIGHT - y;
    const int alph = texture->texture->_alphaValue;
    for (int i = 0; i < sh; i++)
    {
        for (int j = 0; j < sw; j++)
        {
            const int dataPix = texture->texture->GetPixel(texture->texture, sx + j, sy + i);
            if (dataPix != alph)
            {
                _PC_DrawVramPixel(x + j,y + i,__psp_imgC_ToRGB(texture->texture->DecodePixel(texture->texture, dataPix)).color);
            }
        }
    }
}

void PC_DrawSubTextureSize(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h)
{
    if (sw == w && sh == h)
    {
        PC_DrawSubTexture(texture, x, y, sx, sy, sw, sh);
        return;
    }
    int flipX = 0;
    int flipY = 0;
    
    if (w < 0) {
        flipX = 1;
        w = -w;
    }
    if (h < 0) {
        flipY = 1;
        h = -h;
    }
    int x2 = 0, y2 = 0;
    unsigned int sx2 = 0, sy2 = 0;
    int xinc = (sw << 16) / w;
    int yinc = (sh << 16) / h;
    //limites les bornes de la texture
    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;
    if (sx + sw > texture->texture->width && sw > 0)
    {
        long long ratio = (long long)w * 0x10000 / sw;
        sw = texture->texture->width - sx;
        w = sw * ratio / 0x10000;
    }
    if (sy + sh > texture->texture->height && sh > 0)
    {
        long long ratio = (long long)h * 0x10000 / sh;
        sh = texture->texture->height - sy;
        h = sh * ratio / 0x10000;
    }
    //limites les bornes en fonction de la taille de la fenetre
    if(x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if(x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if(y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    const int alph = texture->texture->_alphaValue;
    int row = 0;
    int col = 0;
    while (row < h) {
        x2 = 0;
        col = 0;
        
        sy2 = sy + (flipY ? (sh - 1 - (y2 >> 16)) : (y2 >> 16));
        while (col < w) {
            sx2 = sx + (flipX ? (sw - 1 - (x2 >> 16)) : (x2 >> 16));
            const int dataPix = texture->texture->GetPixel(texture->texture, sx2, sy2);
            
            if (dataPix != alph)
            {
                _PC_DrawVramPixel(x + col,y + row,__psp_imgC_ToRGB(texture->texture->DecodePixel(texture->texture, dataPix)).color);
            }
            
            x2 += xinc;
            col++;
        }
        
        y2 += yinc;
        row++;
    }
}

void PC_DrawSubTextureSizeColored(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h, PC_Color color)
{
    int flipX = 0;
    int flipY = 0;
    
    if (w < 0) {
        flipX = 1;
        w = -w;
    }
    if (h < 0) {
        flipY = 1;
        h = -h;
    }
    int x2 = 0, y2 = 0;
    unsigned int sx2 = 0, sy2 = 0;
    int xinc = (sw << 16) / w;
    int yinc = (sh << 16) / h;
    //limites les bornes de la texture
    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;
    if (sx + sw > texture->texture->width && sw > 0)
    {
        long long ratio = (long long)w * 0x10000 / sw;
        sw = texture->texture->width - sx;
        w = sw * ratio / 0x10000;
    }
    if (sy + sh > texture->texture->height && sh > 0)
    {
        long long ratio = (long long)h * 0x10000 / sh;
        sh = texture->texture->height - sy;
        h = sh * ratio / 0x10000;
    }
    //limites les bornes en fonction de la taille de la fenetre
    if(x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if(x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if(y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    const int alph = texture->texture->_alphaValue;
    int row = 0;
    int col = 0;
    while (row < h) {
        x2 = 0;
        col = 0;
        
        sy2 = sy + (flipY ? (sh - 1 - (y2 >> 16)) : (y2 >> 16));
        while (col < w) {
            sx2 = sx + (flipX ? (sw - 1 - (x2 >> 16)) : (x2 >> 16));
            const int dataPix = texture->texture->GetPixel(texture->texture, sx2, sy2);
            
            if (dataPix != alph)
            {
                int colPix = texture->texture->DecodePixel(texture->texture, dataPix);
                //multiply color
                PC_Color rgb = __psp_imgC_ToRGB(colPix);
                PC_Color colorRes =PC_ColorCreate(rgb.r * color.r / 255, rgb.g * color.g / 255, rgb.b * color.b / 255, 255);
                _PC_DrawVramPixel(x + col,y + row,colorRes.color);
            }
            
            x2 += xinc;
            col++;
        }
        
        y2 += yinc;
        row++;
    }
}

void PC_DrawSubTextureRect(PC_Texture* texture, PC_Rect* src, PC_Rect* dest, PC_Color TextureColorMod)
{
    if (dest == NULL)
        return;
    if (PC_ColorEqual(TextureColorMod,PC_WHITE))
    {
        if (src == NULL)
        {
            PC_DrawSubTexture(texture, dest->x, dest->y, GetWidth(texture), GetHeight(texture), dest->w, dest->h);
            return;
        }
        PC_DrawSubTextureSize(texture, dest->x, dest->y, src->x, src->y, src->w, src->h, dest->w, dest->h);
    }
    else
    {
        if (src == NULL)
        {
            PC_DrawSubTextureSizeColored(texture, dest->x, dest->y, 0, 0, GetWidth(texture), GetHeight(texture), dest->w, dest->h, TextureColorMod);
            return;
        }
        PC_DrawSubTextureSizeColored(texture, dest->x, dest->y, src->x, src->y, src->w, src->h, dest->w, dest->h, TextureColorMod);
    }
}

PC_Color PC_GetPixelColor(PC_Texture* texture, int x, int y)
{
    return (PC_Color){0,0,0,0,0};
}

void PC_SetPixelColor(PC_Texture* texture, int x, int y, PC_Color color)
{
    PC_ErrorScreen("Error: SetPixelColor not implemented");
    //texture->texture->SetPixel(texture, x, y, color.color);
}

PC_Texture* PC_LoadTextureSprite(const char* path,int index)
{
    PC_ErrorScreen("Error: LoadTextureSprite not implemented");
    return NULL;
}