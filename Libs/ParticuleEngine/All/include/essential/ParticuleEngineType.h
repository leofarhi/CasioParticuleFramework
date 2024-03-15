#ifndef PARTICULE_ENGINE_TYPE_H
#define PARTICULE_ENGINE_TYPE_H
#include <stddef.h>

//if not c++ compiler
#if !defined(__cplusplus)
#if !defined(bool)
typedef int bool;
#endif
#if !defined(true)
#define true 1
#define false 0
#endif
#endif

typedef struct Vector2{ //vecteur 2D
    int x;
    int y;
}Vector2;

typedef struct Vector3{ //vecteur 3D
    int x;
    int y;
    int z;
}Vector3;

typedef struct PC_Rect{
    int x;
    int y;
    int w;
    int h;
}PC_Rect;

typedef struct String{
    char* data;
    size_t size;
}String;

typedef struct lambdaFunction{
    void (*function)(void* data);
    void* data;
}lambdaFunction;

lambdaFunction lambdaFunction_create(void *function, void* data);
void lambdaFunction_call(lambdaFunction function);
void lambdaFunction_clear(lambdaFunction function);
bool lambdaFunction_isEmpty(lambdaFunction function);

Vector2 vector2_create(int x, int y);
Vector2* vector2_create_ptr(int x, int y);

Vector3 vector3_create(int x, int y, int z);
Vector3* vector3_create_ptr(int x, int y, int z);

String* string_create(const char* data);

float vector3_distance(Vector3 a, Vector3 b);

#endif