#include <stdio.h>
#include <List.h>
#include <ParticuleEngineEssential.h>
#include <ParticuleEngineGraphic.h>

int main(void)
{
    printf("Hello, World!\n");
    List* list = List_new();
    List_free(list);
    printf("%d\n", truemod(5, 3));
    PC_InitBase(800, 600);
    while (1)
    {
        PC_ClearScreen();
        PC_UpdateInputs();
        PC_UpdateScreen();
    }
    
    PC_QuitBase();
    return 0;
}