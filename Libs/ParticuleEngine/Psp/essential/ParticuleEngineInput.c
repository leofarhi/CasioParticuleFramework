#include "ParticuleEngineOverwrited.h"

Vector2 MousePosition = {0, 0};

struct SceCtrlData __pc_padData;
struct SceCtrlData __pc_oldData;

void PC_UpdateInputs()
{
    __pc_oldData = __pc_padData;
    sceCtrlReadBufferPositive(&__pc_padData,1);
}

bool PC_IsKeyPressed(int key)
{
    bool current = (__pc_padData.Buttons & (unsigned int)key) == (unsigned int)key;
    bool last = (__pc_oldData.Buttons & (unsigned int)key) == (unsigned int)key;
    return current || last; // Currently pressed, and pressed last time
}

bool PC_IsKeyDown(int key)
{
    bool current = (__pc_padData.Buttons & (unsigned int)key) == (unsigned int)key;
    bool last = (__pc_oldData.Buttons & (unsigned int)key) == (unsigned int)key;
    return current && !last; // Currently pressed, not pressed last time
}

bool PC_IsKeyDownWait(int key)
{
    if (PC_IsKeyDown(key))
    {
        while (PC_IsKeyPressed(key))
        {
            PC_UpdateInputs();
        }
        return true;
    }
    return false;
}

bool PC_IsKeyUp(int key)
{
    bool current = (__pc_padData.Buttons & (unsigned int)key) == (unsigned int)key;
    bool last = (__pc_oldData.Buttons & (unsigned int)key) == (unsigned int)key;
    return !current && last; // Not currently pressed, pressed last time
}

bool PC_IsMouseButtonPressed(int button)
{
    return PC_IsKeyPressed(button);
}

bool PC_IsMouseButtonDown(int button)
{
    return PC_IsKeyDown(button);
}

bool PC_IsMouseButtonUp(int button)
{
    return PC_IsKeyUp(button);
}

Vector2 PC_InputAnalog(int stick)
{
    (void)stick;
    Vector2 v = {__pc_padData.Lx-128, -(__pc_padData.Ly-128)};
    return v;
}

bool PC_MouseOver(int x, int y, int w, int h)
{
    return MousePosition.x >= x && MousePosition.x <= x+w && MousePosition.y >= y && MousePosition.y <= y+h;
}

char* PC_GetText()
{
    //C:\pspsdk\psp\sdk\samples\utility\osk
    return scanf_keyboard(PSP_list,"please enter some text","");
}