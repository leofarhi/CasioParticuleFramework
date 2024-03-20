#include "ParticuleEngineOverwrited.h"

List* InputEvents = NULL;
List* InputEventsHeld = NULL;

Vector2 MousePosition = {0,0};

void PC_UpdateInputs(){
    while (InputEvents->size > 0)
    {
        SDL_Event* event = List_pop(InputEvents);
        if (event->type == SDL_QUIT
        #if defined(DEBUG_MODE)
        || (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
        #endif
        )
        {
            exit(0);
        }
        free(event);
    }
    List_clear(InputEvents);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.key.repeat 
            && !(event.type == SDL_QUIT))
            continue; // Ignore key repeats (holding down a key
        SDL_Event* eventCopy = malloc(sizeof(SDL_Event));
        memcpy(eventCopy, &event, sizeof(SDL_Event));
        List_add(InputEvents, eventCopy);
        if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN){
            SDL_Event* eventCopy2 = malloc(sizeof(SDL_Event));
            memcpy(eventCopy2, &event, sizeof(SDL_Event));
            List_add(InputEventsHeld, eventCopy2);
        }
        if (event.type == SDL_KEYUP){
            for (List_node*cur=NULL; ForEach(InputEventsHeld,&cur);)
            {
                SDL_Event* eventHeld = (SDL_Event*)(cur->data);
                if (eventHeld->key.keysym.sym == event.key.keysym.sym){
                    cur = cur->next;
                    List_remove(InputEventsHeld, eventHeld);
                    free(eventHeld);
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONUP){
            for (List_node*cur=NULL; ForEach(InputEventsHeld,&cur);)
            {
                SDL_Event* eventHeld = (SDL_Event*)(cur->data);
                if (eventHeld->button.button == event.button.button){
                    cur = cur->next;
                    List_remove(InputEventsHeld, eventHeld);
                    free(eventHeld);
                }
            }
        }
        if (event.type == SDL_MOUSEMOTION)
        {
            MousePosition.x = event.motion.x/3;
            MousePosition.y = event.motion.y/3;
        }
    }
}

bool PC_IsKeyPressed(int key)
{
    for (List_node*cur=NULL; ForEach(InputEventsHeld,&cur);)
    {
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == key)
        {
            return true;
        }
    }
    return false;
}

bool PC_IsKeyDown(int key)
{
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
	{
		SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == key)
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
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_KEYUP && event->key.keysym.sym == key)
        {
            return true;
        }
    }
    return false;
}

bool PC_IsMouseButtonPressed(int button)
{
    for (List_node*cur=NULL; ForEach(InputEventsHeld,&cur);)
    {
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == button)
        {
            return true;
        }
    }
    return false;
}

bool PC_IsMouseButtonDown(int button)
{
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
    {
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == button)
        {
            return true;
        }
    }
    return false;
}

bool PC_IsMouseButtonUp(int button)
{
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
    {
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_MOUSEBUTTONUP && event->button.button == button)
        {
            return true;
        }
    }
    return false;
}

Vector2 PC_InputAnalog(int stick)
{
    (void)(stick);
    return (Vector2){0,0};
}

bool PC_MouseOver(int x, int y, int w, int h)
{
    return MousePosition.x >= x && MousePosition.x <= x+w && MousePosition.y >= y && MousePosition.y <= y+h;
}

char* PC_GetText()//don't forget to free the returned string
{
    for (List_node*cur=NULL; ForEach(InputEvents,&cur);)
    {
        SDL_Event* event = (SDL_Event*)(cur->data);
        if (event->type == SDL_TEXTINPUT)
        {
            if (event->text.text==NULL || strlen(event->text.text)==0)
                return NULL;
            char* text = (char*)malloc(sizeof(char)*strlen(event->text.text)+1);
            strcpy(text, event->text.text);
            return text;
        }
    }
    return NULL;
}
