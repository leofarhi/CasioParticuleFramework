#include "ParticuleEngineOverwrited.h"

List* InputEvents = NULL;
Vector2 MousePosition = {0, 0};

void PC_UpdateInputs()
{
    //clearevents();
    while (InputEvents->size > 0)
    {
        free(List_pop(InputEvents));
    }
    key_event_t ev;
    while((ev = pollevent()).type != KEYEV_NONE)
    {
        PC_GINT_keyinfo *key = malloc(sizeof(PC_GINT_keyinfo));
        key->type = ev.type;
        key->key = ev.key;
        List_add(InputEvents, key);
    }
}

bool PC_IsKeyPressed(int key)
{
    return keydown(key);
}

bool PC_IsKeyDown(int key)
{
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
    {
        PC_GINT_keyinfo* event = (PC_GINT_keyinfo*)(cur->data);
        if (event->type == KEYEV_DOWN && event->key == key)
        {
            return true;
        }
    }
    return false;
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
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
    {
        PC_GINT_keyinfo* event = (PC_GINT_keyinfo*)(cur->data);
        if (event->type == KEYEV_UP && event->key == key)
        {
            return true;
        }
    }
    return false;
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
    (void)(stick);
    return (Vector2){0, 0};
}

bool PC_MouseOver(int x, int y, int w, int h)
{
    return MousePosition.x >= x && MousePosition.x <= x+w && MousePosition.y >= y && MousePosition.y <= y+h;
}

char* PC_GetText()
{
    return scanf_keyboard();
}