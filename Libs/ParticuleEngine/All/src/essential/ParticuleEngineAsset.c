#include "../../include/essential/ParticuleEngineGraphic.h"

List* __pc_AllResources = NULL;

#ifndef OVERWRITE_INIT_ASSET
void (*__pc_afterInitAsset)() = NULL;
void (*__pc_beforeQuitAsset)() = NULL;
#endif

bool __pc_pathcmp(const char* path1, const char* path2)
{
    char *path1_lower = malloc(strlen(path1) + 1);
    strcpy(path1_lower, path1);
    char *path2_lower = malloc(strlen(path2) + 1);
    strcpy(path2_lower, path2);
    PC_toLower(&path1_lower);
    PC_toLower(&path2_lower);
    bool result = strcmp(path1_lower, path2_lower) == 0;
    free(path1_lower);
    free(path2_lower);
    return result;
}

void PC_InitAsset()
{
    __pc_AllResources = List_new();
    if (__pc_afterInitAsset != NULL)
        __pc_afterInitAsset();
}

void PC_QuitAsset()
{
    if (__pc_beforeQuitAsset != NULL)
        __pc_beforeQuitAsset();
    while (__pc_AllResources->size > 0)
    {
        VirtualFile* file = (VirtualFile*)List_pop(__pc_AllResources);
        free(file->path);
        if (file->free != NULL && file->loaded)
        {
            file->free(file);
        }
        free(file);
    }
    List_free(__pc_AllResources);
}

VirtualFile* PC_LoadResource(unsigned char* path)
{
    for (List_node*cur=NULL; ForEach(__pc_AllResources,&cur);)
    {
        VirtualFile* file = (VirtualFile*)(cur->data);
        if (__pc_pathcmp((const char *)file->path, (const char *)path))
        {
            if (!file->loaded)
            {
                file->loaded = true;
                if (file->load != NULL)
                    file->load(file);
            }
            return file;
        }
    }
    return NULL;
}

void PC_UnloadResource(VirtualFile* file)
{
    if (file->free != NULL && file->loaded)
    {
        file->free(file);
        file->loaded = false;
    }
}

void PC_UnloadResourcePath(unsigned char* path)
{
    for (List_node*cur=NULL; ForEach(__pc_AllResources,&cur);)
    {
        VirtualFile* file = (VirtualFile*)(cur->data);
        if (__pc_pathcmp((const char *)file->path, (const char *)path))
        {
            PC_UnloadResource(file);
            return;
        }
    }
}


VirtualFile* PC_AddResource(unsigned char* path, void* data, int type, void (*load)(VirtualFile* file), void (*free)(VirtualFile* file))
{
    VirtualFile* file = malloc(sizeof(VirtualFile));
    file->path = malloc(strlen((const char *)path) + 1);
    strcpy(file->path, (const char *)path);
    file->data = data;
    file->load = load;
    file->free = free;
    file->loaded = false;
    file->type = type;
    List_add(__pc_AllResources, file);
    return file;
}

void PC_RemoveResource(unsigned char* path)
{
    for (List_node*cur=NULL; ForEach(__pc_AllResources,&cur);)
    {
        VirtualFile* file = (VirtualFile*)(cur->data);
        if (__pc_pathcmp((const char *)file->path,(const char *) path))
        {
            List_remove(__pc_AllResources, cur);
            free(file->path);
            if (file->free != NULL && file->loaded)
            {
                file->free(file);
            }
            free(file);
            return;
        }
    }
}