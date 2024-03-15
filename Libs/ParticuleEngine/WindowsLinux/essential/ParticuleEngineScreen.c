#include "ParticuleEngineOverwrited.h"

SDL_Window *__sdl_window = NULL;
SDL_Renderer *__sdl_renderer = NULL;

int SCREEN_WIDTH = 396;
int SCREEN_HEIGHT = 224;

#ifdef DIST_WINDOWS
void errx(int exitcode, const char *format, const char* error)
{
    fprintf(stderr, format, error);
    exit(exitcode);
}
#endif

void __PC_ResetDirectory()
{
    // Obtenez le chemin absolu du programme
    char *path = SDL_GetBasePath();
    if (path == NULL)
        errx(EXIT_FAILURE, "error SDL_GetBasePath %s", SDL_GetError());

    // Changez le répertoire de travail
    if (chdir(path) != 0)
        errx(EXIT_FAILURE, "error chdir %s", SDL_GetError());

    // Libérez la mémoire
    //printf("path: %s\n", path);
    SDL_free(path);
}

void PC_InitBase(int width, int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "error Init %s", SDL_GetError());
        // Creates a window.
    __sdl_window = SDL_CreateWindow("Engine", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);// | SDL_WINDOW_RESIZABLE);
    if (__sdl_window == NULL)
        errx(EXIT_FAILURE, "error window %s", SDL_GetError());

    __PC_ResetDirectory();

    // Creates a renderer.
    __sdl_renderer = SDL_CreateRenderer(__sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (__sdl_renderer == NULL)
        errx(EXIT_FAILURE, "error render %s", SDL_GetError());

    // Initializes the image library.
    if (IMG_Init(IMG_INIT_PNG) == 0)
        errx(EXIT_FAILURE, "error IMG_Init %s", IMG_GetError());

    // Initializes the font library.
    if (TTF_Init() == -1)
        errx(EXIT_FAILURE, "error TTF_Init %s", TTF_GetError());
    //enable Alpha Blending
    SDL_SetRenderDrawBlendMode(__sdl_renderer, SDL_BLENDMODE_BLEND);
    // Dispatches the events.
    InputEvents = List_new();
    InputEventsHeld = List_new();
}

void PC_QuitBase()
{
    // Quits the font library.
    TTF_Quit();

    // Quits the image library.
    IMG_Quit();

    // Destroys the renderer.
    SDL_DestroyRenderer(__sdl_renderer);

    // Destroys the window.
    SDL_DestroyWindow(__sdl_window);

    // Quits the SDL.
    SDL_Quit();
    while (InputEvents->size > 0)
        free(List_pop(InputEvents));
    while (InputEventsHeld->size > 0)
        free(List_pop(InputEventsHeld));
    List_free(InputEvents);
    List_free(InputEventsHeld);
}

void PC_ClearScreen()
{
    SDL_SetRenderDrawColor(__sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__sdl_renderer);
}

void PC_ClearScreenColor(PC_Color color)
{
    SDL_SetRenderDrawColor(__sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(__sdl_renderer);
}

void PC_UpdateScreen()
{
    SDL_RenderPresent(__sdl_renderer);
}

void PC_SelectScreen(int screen)
{
    // Nothing to do.
    (void)screen;
}

int PC_GetTicks()
{
    return SDL_GetTicks();
}