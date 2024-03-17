#include <stdio.h>
#include <List.h>
#include <ParticuleEngineOverwrited.h>
#include <ParticuleEngineGraphic.h>

int main(void)
{
    PC_InitBase(800, 600);
    PC_SetMainFont(PC_LoadFont("assets/Fonts/Calibri.ttf", 16));
    int key;
    #ifdef DIST_WINDOWS
    key = SDLK_a;
    #elif defined DIST_CASIO_CG
    key = KEY_MENU;
    #endif
    int key2;
    #ifdef DIST_WINDOWS
    key2 = SDLK_b;
    #elif defined DIST_CASIO_CG
    key2 = KEY_EXE;
    #endif
    while (!PC_IsKeyDown(key))
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_DrawText((const unsigned char *)"Hello, World!", 100, 0, PC_ColorCreate(255, 255, 255, 255),MainFont);
        PC_DrawRect(0, 0, 50, 50, PC_ColorCreate(255, 0, 0, 255));
        PC_DrawFilledRect(50, 0, 50, 50, PC_ColorCreate(0, 255, 0, 255));
        PC_DrawLine(0, 50, 49, 50, PC_ColorCreate(0, 0, 255, 255));
        PC_DrawPixel(50, 51, PC_ColorCreate(255, 255, 255, 255));

        if (PC_ColorEqual(PC_ColorCreate(1,2,3,4), PC_ColorCreate(1,2,3,4)) 
            && PC_ColorEqual(PC_ColorCreate(1,5,3,0), PC_ColorCreate(1,2,3,0))
            && !PC_ColorEqual(PC_ColorCreate(1,2,3,4), PC_ColorCreate(1,2,3,5))
            && !PC_ColorEqual(PC_ColorCreate(1,3,3,5), PC_ColorCreate(1,2,3,5))
            )
            PC_DrawText((const unsigned char *)"Colors comparison works!", 100, 80, PC_ColorCreate(255, 255, 255, 255),MainFont);

        if (PC_IsKeyDown(key2))
            PC_DrawText((const unsigned char *)"You pressed the key!", 100, 20, PC_ColorCreate(255, 255, 255, 255),MainFont);
        if (PC_IsKeyUp(key2))
            PC_DrawText((const unsigned char *)"You released the key!", 100, 40, PC_ColorCreate(255, 255, 255, 255),MainFont);
        if (PC_IsKeyPressed(key2))
            PC_DrawText((const unsigned char *)"You are holding the key!", 100, 60, PC_ColorCreate(255, 255, 255, 255),MainFont);
        char str[150] = "Ce TeXtE eSt En MiNuScUlE oU eN mAjUsCuLe ";
        char *strPtr = str;
        PC_toLower(&strPtr);
        PC_DrawText((const unsigned char *)str, 0, 100, PC_ColorCreate(255, 255, 255, 255),MainFont);
        PC_toUpper(&strPtr);
        PC_DrawText((const unsigned char *)str, 0, 120, PC_ColorCreate(255, 255, 255, 255),MainFont);

        char str2[150];
        sprintf(str2, "The time is %ld", (long int)PC_GetTicks());
        PC_DrawText((const unsigned char *)str2, 0, 140, PC_ColorCreate(255, 255, 255, 255),MainFont);
        sprintf(str2, "The delta time is %ld", (long int)PC_DeltaTime());
        PC_DrawText((const unsigned char *)str2, 0, 160, PC_ColorCreate(255, 255, 255, 255),MainFont);
        sprintf(str2, "The fps is %ld", (long int)PC_FPS());
        PC_DrawText((const unsigned char *)str2, 0, 180, PC_ColorCreate(255, 255, 255, 255),MainFont);
        sprintf(str2, "Mouse position: %d, %d", MousePosition.x, MousePosition.y);
        PC_DrawText((const unsigned char *)str2, 0, 200, PC_ColorCreate(255, 255, 255, 255),MainFont);
        PC_UpdateScreen();
    }
    PC_IsKeyDownWait(key);
    PC_Texture* texture = PC_LoadTexture("assets/Images/ui.png");
    int i = 0;
    while (!PC_IsKeyDown(key))
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_DrawTexture(texture, -(i%GetWidth(texture)), -(i/GetWidth(texture))*50);
        i+=20;
        i = i%(GetWidth(texture)*(GetHeight(texture)/50));
        PC_UpdateScreen();
        PC_Delay(50);
    }
    PC_IsKeyDownWait(key);
    PC_FreeTexture(texture);
    for (size_t i = 0; i < 40; i++)
    {
        PC_FreeTexture(PC_LoadTexture("assets/Images/ui.png"));
    }
    texture = PC_LoadTexture("assets/Images/ui.png");
    while (!PC_IsKeyDown(key))
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_DrawSubTexture(texture, 0, 0, i%GetWidth(texture), (i/GetWidth(texture))*50, 50, 50);
        PC_DrawSubTextureSize(texture, 100, 0, i%GetWidth(texture), (i/GetWidth(texture))*50, 50, 50, 100, 100);
        PC_DrawSubTextureSizeColored(texture, 0, 100, i%GetWidth(texture), (i/GetWidth(texture))*50, 50, 50, 100, 100,PC_ColorCreate(255, 0, 0, 255));
        i+=10;
        i = i%(GetWidth(texture)*(GetHeight(texture)/50));
        PC_UpdateScreen();
        PC_Delay(25);
    }
    PC_FreeTexture(texture);
    PC_FreeFont(MainFont);
    PC_QuitBase();
    printf("Goodbye, World!\n");
    return 0;
}