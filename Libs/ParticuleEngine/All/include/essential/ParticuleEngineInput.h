#ifndef PARTICULE_ENGINE_INPUT_H
#define PARTICULE_ENGINE_INPUT_H
#include "ParticuleEngineGraphic.h"

extern Vector2 MousePosition;

void PC_UpdateInputs();
//Updates the inputs

int PC_GetInputEvent();
//Returns the first input event in the list

bool PC_IsKeyPressed(int key);
//Checks if a key is pressed (Holding down)
//key is the key to check

bool PC_IsKeyDown(int key);
//Checks if a key is down (Pressed once)

bool PC_IsKeyDownWait(int key);
//Checks if a key is down (Pressed once) and waits until the key is released

bool PC_IsKeyUp(int key);
//Checks if a key is up (Released)

bool PC_IsMouseButtonPressed(int button);
//Checks if a mouse button is pressed (Holding down)

bool PC_IsMouseButtonDown(int button);
//Checks if a mouse button is down (Pressed once)

bool PC_IsMouseButtonUp(int button);
//Checks if a mouse button is up (Released)

Vector2 PC_InputAnalog(int stick);

bool PC_MouseOver(int x, int y, int w, int h);

char* PC_GetText();

#endif