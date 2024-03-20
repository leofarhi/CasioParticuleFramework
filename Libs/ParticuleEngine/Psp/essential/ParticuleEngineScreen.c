#include "ParticuleEngineOverwrited.h"

struct __pc_VramDisplay __pc_vram;
long long int __pc_lastTime = 0;

PSP_MODULE_INFO(PROJECT_NAME, 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void __pc_UpdateVramDisplay()
{
    sceGuSwapBuffers();
    __pc_vram.unknown = 0;
    sceDisplayGetFrameBuf((void**)&__pc_vram.vram32, &__pc_vram.bufferwidth, &__pc_vram.pixelformat, __pc_vram.unknown);
    __pc_vram.vram16 = (u16*) __pc_vram.vram32;
    sceGuSwapBuffers();
}

void PC_InitBase(int width, int height)
{
    (void)width;
    (void)height;
    SetupCallbacks();
    initGraphics(PSP_list);

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    //sceDisplaySetFrameBuf(__pc_vram.vram32, __pc_vram.bufferwidth, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
    __pc_UpdateVramDisplay();
}

void PC_QuitBase()
{
    termGraphics();
    sceKernelExitGame();
}

void PC_ClearScreen()
{
    __pc_UpdateVramDisplay();
    for (size_t i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
        _PC_DrawVramPixel(i % SCREEN_WIDTH, i / SCREEN_WIDTH, 0);
    
}

void PC_ClearScreenColor(PC_Color color)
{
    __pc_UpdateVramDisplay();
    for (size_t i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
        _PC_DrawVramPixel(i % SCREEN_WIDTH, i / SCREEN_WIDTH, color.color);
}

void PC_UpdateScreen()
{
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
    __pc_lastTime = PC_GetTicks();
}

void PC_SelectScreen(int screen)
{
    (void)screen;
}

long long int PC_GetTicks()
{
    return sceKernelGetSystemTimeWide();
}

long long int PC_DeltaTime()
{
    long long int currentTime = PC_GetTicks();
    long long int deltaTime = currentTime - __pc_lastTime;
    __pc_lastTime = currentTime;
    return deltaTime;
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
    sceKernelDelayThread(ms);
}

void PC_ErrorScreen(const char* error){
    pspDebugScreenInit();
	pspDebugScreenPrintf(error);
    pspDebugScreenPrintf("\nPress start to exit");
    PC_IsKeyDownWait(PSP_CTRL_START);
    while (!PC_IsKeyDown(PSP_CTRL_START))
    {
        PC_UpdateInputs();
        PC_UpdateScreen();
    }
    sceKernelExitGame();
}

void PC_DebugScreen(const char* str)
{
    pspDebugScreenInit();
    pspDebugScreenPrintf(str);
    pspDebugScreenPrintf("\nPress start");
    PC_IsKeyDownWait(PSP_CTRL_START);
    while (!PC_IsKeyDown(PSP_CTRL_START))
    {
        PC_UpdateInputs();
        PC_UpdateScreen();
    }
    PC_IsKeyDownWait(PSP_CTRL_START);
}