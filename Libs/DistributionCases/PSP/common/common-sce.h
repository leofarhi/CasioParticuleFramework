#ifndef COMMON_GL_H_INCLUDED
#define COMMON_GL_H_INCLUDED

#if __cplusplus__
extern "C"{
#endif
#include <stdio.h>

// Define PSP Width / Height
#define PSP_BUF_WIDTH (512)
#define PSP_SCR_WIDTH (480)
#define PSP_SCR_HEIGHT (272)

FILE* stbi__fopen(char const* filename, char const* mode);

unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm);
void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm);

void initGraphics(void* list);
void startFrame(void* list);
void endFrame();
void termGraphics();


#if __cplusplus__
};
#endif

#endif