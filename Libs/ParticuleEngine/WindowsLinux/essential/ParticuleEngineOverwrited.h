#ifndef PARTICULE_ENGINE_OVERWRITED_H
#define PARTICULE_ENGINE_OVERWRITED_H

#include "../../../All/include/essential/ParticuleEngineEssential.h"

#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#if defined(DIST_WINDOWS) || defined(DIST_LINUX)
#include <unistd.h>
#endif
#if defined(DIST_LINUX)
#include <err.h>
#endif
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Screen
extern SDL_Window *__sdl_window;
extern SDL_Renderer *__sdl_renderer;


extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

#ifdef DIST_WINDOWS
void errx(int exitcode, const char *format, const char * error);
#endif

//Input
extern List* InputEvents;
extern List* InputEventsHeld;

//Font
typedef struct PC_Font
{
    TTF_Font* font;
} PC_Font;

//Texture
typedef struct PC_Texture
{
    SDL_Texture* texture;
    SDL_Surface* surface;
    int width;
    int height;
} PC_Texture;

#define OVERWRITE_SCREEN_SIZE
#define OVERWRITE_STRUCT_FONT
#define OVERWRITE_STRUCT_TEXTURE

#include "../../../All/include/essential/ParticuleEngineGraphic.h"

#endif