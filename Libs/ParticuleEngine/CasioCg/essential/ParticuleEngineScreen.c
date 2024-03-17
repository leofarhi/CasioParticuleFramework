#include "ParticuleEngineOverwrited.h"

clock_t __pc_lastTime = 0;

void PC_InitBase(int width, int height)
{
    (void)width;
    (void)height;
    InputEvents = List_new();
    PC_InitAsset();
}

void PC_QuitBase()
{
    while (InputEvents->size > 0)
        free(List_pop(InputEvents));
    List_free(InputEvents);
    PC_QuitAsset();
}

void PC_ClearScreen()
{
    dclear(C_BLACK);
}

void PC_ClearScreenColor(PC_Color color)
{
    dclear(color.color);
}

void PC_UpdateScreen()
{
    dupdate();
    __pc_lastTime = PC_GetTicks();
}

void PC_SelectScreen(int screen)
{
    //Do nothing
    (void)screen;
}

long long int PC_GetTicks()
{
    return clock();
}

long long int PC_DeltaTime()
{
    return PC_GetTicks() - __pc_lastTime;
}

long long int PC_FPS()
{
    long long int time = PC_DeltaTime();
    long long int fps = LONG_MAX;
    if (time != 0)
        fps = fps / time;
    return fps/1000;
}

void PC_Delay(long long int ms)
{
    sleep_ms(ms);
}

void PC_ErrorScreen(const char* error)
{
    PC_IsKeyDownWait(KEY_EXIT);
    PC_Font* font = PC_LoadFont("", 0);
    while (!PC_IsKeyDown(KEY_EXIT))
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_DrawText((const unsigned char *)error, 0, 0, PC_ColorCreate(255, 255, 255, 255), font);
        PC_DrawText((const unsigned char *)"Press EXIT to quit", 0, 20, PC_ColorCreate(255, 255, 255, 255), font);
        PC_UpdateScreen();
    }
    PC_FreeFont(font);
    exit(0);
}

void PC_DebugScreen(const char* str)
{
    PC_IsKeyDownWait(KEY_EXIT);
    PC_Font* font = PC_LoadFont("", 0);
    while (!PC_IsKeyDown(KEY_EXIT))
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_DrawText((const unsigned char *)str, 0, 0, PC_ColorCreate(255, 255, 255, 255), font);
        PC_DrawText((const unsigned char *)"Press EXIT", 0, 20, PC_ColorCreate(255, 255, 255, 255), font);
        PC_UpdateScreen();
    }
    PC_FreeFont(font);
    PC_IsKeyDownWait(KEY_EXIT);
}