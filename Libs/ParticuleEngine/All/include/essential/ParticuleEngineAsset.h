#ifndef PARTICULE_ENGINE_ASSET_H
#define PARTICULE_ENGINE_ASSET_H
#include "ParticuleEngineGraphic.h"

typedef struct VirtualFile VirtualFile;
typedef struct VirtualFile
{
    char* path;
    void* data;
    void (*load)(VirtualFile* file);
    void (*free)(VirtualFile* file);
    bool loaded;
    int type;
} VirtualFile;

extern List* __pc_AllResources;

extern void (*__pc_afterInitAsset)();
extern void (*__pc_beforeQuitAsset)();

void PC_InitAsset();

void PC_QuitAsset();

VirtualFile* PC_LoadResource(unsigned char* path);

void PC_UnloadResource(VirtualFile* file);

void PC_UnloadResourcePath(unsigned char* path);

VirtualFile* PC_AddResource(unsigned char* path, void* data, int type, void (*load)(VirtualFile* file), void (*free)(VirtualFile* file));

void PC_RemoveResource(unsigned char* path);

#endif