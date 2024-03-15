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

PC_Texture* PC_LoadSubTextureSize(const char* path, int sx, int sy, int sw, int sh)
{
    PC_Texture* texture = PC_LoadTexture(path);
    if (texture == NULL)
        return NULL;
    SDL_Surface* surface = texture->surface;
    SDL_Surface* subSurface = SDL_CreateRGBSurfaceWithFormat(0, sw, sh, surface->format->BitsPerPixel, surface->format->format);
    if (subSurface == NULL) {
        printf("Erreur lors de la création de la sous-surface : %s\n", SDL_GetError());
        return NULL;
    }

    // Copie de la sous-surface avec conservation du canal alpha
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
    SDL_SetSurfaceBlendMode(subSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect srcRect = {sx, sy, sw, sh};
    SDL_BlitSurface(surface, &srcRect, subSurface, NULL);
    //Free last texture
    SDL_DestroyTexture(texture->texture);
    //Free last surface
    SDL_FreeSurface(texture->surface);
    texture->surface = subSurface;


    // Création de la texture
    texture->texture = SDL_CreateTextureFromSurface(__sdl_renderer, subSurface);
    if (texture->texture == NULL)
        errx(EXIT_FAILURE, "error SDL_CreateTextureFromSurface %s", SDL_GetError());
    texture->width = subSurface->w;
    texture->height = subSurface->h;
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
    SDL_Rect rect = { x, y, abs(w), abs(h) };
    SDL_Rect rect2 = { sx, sy, sw, sh };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (w < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (h < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyEx(__sdl_renderer, texture->texture, &rect2, &rect, 0, NULL, flip);
}

void PC_DrawSubTextureSizeColored(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h, PC_Color color)
{
    SDL_Rect rect = { x, y, abs(w), abs(h) };
    SDL_Rect rect2 = { sx, sy, sw, sh };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (w < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (h < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }
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