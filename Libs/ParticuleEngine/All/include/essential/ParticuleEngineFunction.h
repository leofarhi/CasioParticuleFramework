#ifndef PARTICULE_ENGINE_FUNCTION_H
#define PARTICULE_ENGINE_FUNCTION_H

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#define truemod(a,b) ((a % b + b) % b)

#define clamp(a, b, c) (min(max(a, b), c))
#define lerp(a, b, t) (a + (b - a) * t)
#define invlerp(a, b, t) ((t - a) / (b - a))

#define int_to_ptr(a) ((void*)(size_t)(a))
#define ptr_to_int(a) ((int)(size_t)(a))

#endif