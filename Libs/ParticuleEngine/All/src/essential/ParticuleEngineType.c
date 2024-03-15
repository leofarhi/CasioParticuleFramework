#include "../../include/essential/ParticuleEngineEssential.h"

lambdaFunction lambdaFunction_create(void *function, void* data)
{
    lambdaFunction result;
    result.function = (void (*)(void*))function;
    result.data = data;
    return result;
}

void lambdaFunction_call(lambdaFunction function)
{
    if (function.function != NULL)
    {
        function.function(function.data);
    }
}

void lambdaFunction_clear(lambdaFunction function)
{
    function.function = NULL;
    function.data = NULL;
    (void)function;
}

bool lambdaFunction_isEmpty(lambdaFunction function)
{
    return function.function == NULL;
}

String* string_create(const char* data)
{
    String* str = malloc(sizeof(String));
    str->size = strlen(data);
    str->data = malloc(str->size + 1);
    strcpy(str->data, data);
    return str;
}

Vector2 vector2_create(int x, int y){
    Vector2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

Vector2* vector2_create_ptr(int x, int y){
    Vector2* vec = malloc(sizeof(Vector2));
    vec->x = x;
    vec->y = y;
    return vec;
}

Vector3 vector3_create(int x, int y, int z)
{
    Vector3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

Vector3* vector3_create_ptr(int x, int y, int z)
{
    Vector3* vec = malloc(sizeof(Vector3));
    vec->x = x;
    vec->y = y;
    vec->z = z;
    return vec;
}

float vector3_distance(Vector3 a, Vector3 b)
{
    return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
}