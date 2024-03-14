#include "EventDataSystem.h"

EventDataSystem* eventDataSystem = NULL;

void initEventDataSystem(){
    if (eventDataSystem != NULL)
        return;
    eventDataSystem = malloc(sizeof(EventDataSystem));
    eventDataSystem->selectedUI = NULL;
    #if defined(WIN_MODE)  || defined(LINUX_MODE)
    eventDataSystem->useController = false;

    eventDataSystem->mouseLeftClick = SDL_BUTTON_LEFT;
    eventDataSystem->mouseRightClick = SDL_BUTTON_RIGHT;
    eventDataSystem->submitButton = SDLK_RETURN;
    eventDataSystem->cancelButton = SDLK_ESCAPE;
    eventDataSystem->deleteButton = SDLK_BACKSPACE;

    eventDataSystem->upButton = SDLK_UP;
    eventDataSystem->downButton = SDLK_DOWN;
    eventDataSystem->leftButton = SDLK_LEFT;
    eventDataSystem->rightButton = SDLK_RIGHT;
    #elif defined(PSP_MODE)
    eventDataSystem->useController = true;

    eventDataSystem->submitButton = PSP_CTRL_CROSS;
    eventDataSystem->cancelButton = PSP_CTRL_CIRCLE;
    eventDataSystem->mouseLeftClick = PSP_CTRL_CROSS;
    eventDataSystem->mouseRightClick = PSP_CTRL_SQUARE;
    eventDataSystem->deleteButton = PSP_CTRL_TRIANGLE;

    eventDataSystem->upButton = PSP_CTRL_UP;
    eventDataSystem->downButton = PSP_CTRL_DOWN;
    eventDataSystem->leftButton = PSP_CTRL_LEFT;
    eventDataSystem->rightButton = PSP_CTRL_RIGHT;
    #elif defined(NDS_MODE)
    eventDataSystem->useController = true;

    eventDataSystem->submitButton = KEY_A;
    eventDataSystem->cancelButton = KEY_B;
    eventDataSystem->mouseLeftClick = KEY_A;
    eventDataSystem->mouseRightClick = KEY_Y;
    eventDataSystem->deleteButton = KEY_X;

    eventDataSystem->upButton = KEY_UP;
    eventDataSystem->downButton = KEY_DOWN;
    eventDataSystem->leftButton = KEY_LEFT;
    eventDataSystem->rightButton = KEY_RIGHT;
    #elif defined(CG_MODE) || defined(FX_MODE)
    eventDataSystem->useController = true;
    
    eventDataSystem->submitButton = KEY_EXE;
    eventDataSystem->cancelButton = KEY_EXIT;
    eventDataSystem->mouseLeftClick = KEY_EXE;
    eventDataSystem->mouseRightClick = KEY_NEG;
    eventDataSystem->deleteButton = KEY_DEL;

    eventDataSystem->upButton = KEY_UP;
    eventDataSystem->downButton = KEY_DOWN;
    eventDataSystem->leftButton = KEY_LEFT;
    eventDataSystem->rightButton = KEY_RIGHT;
    #endif
}