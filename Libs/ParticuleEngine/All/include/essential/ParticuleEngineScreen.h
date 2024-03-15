#ifndef PARTICULE_ENGINE_SCREEN_H
#define PARTICULE_ENGINE_SCREEN_H
#include "ParticuleEngineGraphic.h"


#ifndef OVERWRITE_SCREEN_SIZE
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#endif

void PC_InitBase(int width, int height);
//Initializes the engine

void PC_QuitBase();
//Quits the engine

void PC_ClearScreen();
//Clears the screen

typedef struct PC_Color PC_Color;
void PC_ClearScreenColor(PC_Color color);
//Clears the screen with a color

void PC_UpdateScreen();
//Updates the screen

void PC_SelectScreen(int screen);
//Selects the screen to draw on

int PC_GetTicks();

#endif