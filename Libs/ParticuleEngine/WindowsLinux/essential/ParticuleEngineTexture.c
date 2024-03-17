#include "ParticuleEngineOverwrited.h"

int GetWidth(PC_Texture* texture)
{
    return texture->width;
}

int GetHeight(PC_Texture* texture)
{
    return texture->height;
}

PC_Texture* PC_LoadTexture(const char* path)
{
    PC_Texture* texture = malloc(sizeof(PC_Texture));
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "error IMG_Load %s", IMG_GetError());
    texture->texture = SDL_CreateTextureFromSurface(__sdl_renderer, surface);
    if (texture->texture == NULL)
        errx(EXIT_FAILURE, "error SDL_CreateTextureFromSurface %s", SDL_GetError());
    texture->width = surface->w;
    texture->height = surface->h;
    //convert to RGBA
    texture->surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0); 
    SDL_FreeSurface(surface);
    return texture;
}

PC_Texture* PC_CreateTexture(int width, int height)
{
    PC_Texture* texture = malloc(sizeof(PC_Texture));
    texture->texture = SDL_CreateTexture(__sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    texture->width = width;
    texture->height = height;
    texture->surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    return texture;
}

void PC_FreeTexture(PC_Texture* texture)
{
    SDL_DestroyTexture(texture->texture);
    SDL_FreeSurface(texture->surface);
    free(texture);
}

void PC_DrawTexture(PC_Texture* texture, int x, int y)
{
    SDL_Rect rect = {x, y, texture->width, texture->height};
    SDL_RenderCopy(__sdl_renderer, texture->texture, NULL, &rect);
}

void PC_DrawSubTexture(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh)
{
    if (sx + sw > texture->width)
        sw = texture->width - sx;
    if (sy + sh > texture->height)
        sh = texture->height - sy;
    SDL_Rect rect = {x, y, sw, sh};
    SDL_Rect rect2 = {sx, sy, sw, sh};
    SDL_RenderCopy(__sdl_renderer, texture->texture, &rect2, &rect);
}

void PC_DrawSubTextureSize(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h)
{
    if (sw == w && sh == h)
    {
        PC_DrawSubTexture(texture, x, y, sx, sy, sw, sh);
        return;
    }
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (w < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (h < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }
    w = abs(w);
    h = abs(h);
    if (sx + sw > texture->width && w > 0)
    {
        float ratio = (float)w / sw;
        sw = texture->width - sx;
        w = sw * ratio;
    }
    if (sy + sh > texture->height && h > 0)
    {
        float ratio = (float)h / sh;
        sh = texture->height - sy;
        h = sh * ratio;
    }
    SDL_Rect rect = { x, y, w, h };
    SDL_Rect rect2 = { sx, sy, sw, sh };

    SDL_RenderCopyEx(__sdl_renderer, texture->texture, &rect2, &rect, 0, NULL, flip);
}

void PC_DrawSubTextureSizeColored(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h, PC_Color color)
{
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (w < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (h < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }
    w = abs(w);
    h = abs(h);
    if (sx + sw > texture->width && sw > 0)
    {
        float ratio = (float)w / sw;
        sw = texture->width - sx;
        w = sw * ratio;
    }
    if (sy + sh > texture->height && sh > 0)
    {
        float ratio = (float)h / sh;
        sh = texture->height - sy;
        h = sh * ratio;
    }
    SDL_Rect rect = { x, y, w, h };
    SDL_Rect rect2 = { sx, sy, sw, sh };
    SDL_SetTextureColorMod(texture->texture, color.r, color.g, color.b);
    SDL_RenderCopyEx(__sdl_renderer, texture->texture, &rect2, &rect, 0, NULL, flip);
    SDL_SetTextureColorMod(texture->texture, 255, 255, 255);
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
    if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
        return PC_ColorCreate(0, 0, 0, 0);
    SDL_Surface* surface = texture->surface;
    Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
    PC_Color color;
    SDL_GetRGBA(pixel, surface->format, &color.r, &color.g, &color.b, &color.a);
    return color;
}

void PC_SetPixelColor(PC_Texture* texture, int x, int y, PC_Color color)
{
    if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
        return;
    SDL_Surface* surface = texture->surface;
    Uint32 pixel = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
    ((Uint32*)surface->pixels)[y * surface->w + x] = pixel;
}

PC_Texture* PC_LoadTextureSprite(const char* path,int index)
{
    char *path2 = malloc(strlen(path) + 30);
    sprintf(path2, "%s/%d.png", path, index);
    PC_Texture* texture = PC_LoadTexture(path2);
    free(path2);
    return texture;
}