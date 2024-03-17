#include "../../include/essential/ParticuleEngineEssential.h"

void PC_toLower(char** str)
{
    int len = strlen(*str);
    for (int i = 0; i < len; i++)
    {
        if ((*str)[i] >= 'A' && (*str)[i] <= 'Z')
        {
            (*str)[i] = (*str)[i] + ('a' - 'A');
        }
    }
}

void PC_toUpper(char** str)
{
    int len = strlen(*str);
    for (int i = 0; i < len; i++)
    {
        if ((*str)[i] >= 'a' && (*str)[i] <= 'z')
        {
            (*str)[i] = (*str)[i] - ('a' - 'A');
        }
    }
}