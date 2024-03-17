#include "Resources.h"

typedef enum
{
    PC_TEXTURE,
    PC_FONT,
    PC_SOUND,
    PC_MUSIC,
    PC_TEXT,
    PC_OTHER
} PC_ResourceType;

void __pc_loadfile(VirtualFile* file)
{
    file->loaded = false;
}

void __pc_freeTexture(VirtualFile* file)
{
    PC_Texture* texture = (PC_Texture*)file->data;
    texture->isAllocated = true;
    PC_FreeTexture(texture);
}

VirtualFile* __pc_AddTexture(unsigned char* path, bopti_image_t* texture,char* IsExternal)
{
    char* nw_path = malloc(strlen((const char *)path) + 1);
    strcpy(nw_path, (const char *)path);
    PC_Texture* tex = (PC_Texture*)malloc(sizeof(PC_Texture));
    if (IsExternal != NULL)
    {
        tex->texture = NULL;
        tex->IsExternalPath = malloc(strlen(IsExternal) + 1);
        strcpy(tex->IsExternalPath, IsExternal);
    }
    else
    {
        tex->texture = texture;
        tex->IsExternalPath = NULL;
    }
    return PC_AddResource((unsigned char *)nw_path, tex, PC_TEXTURE, &__pc_loadfile, &__pc_freeTexture);
}

void __pc_freeFont(VirtualFile* file)
{
    PC_Font* font = (PC_Font*)file->data;
    free(font);
}

VirtualFile* __pc_AddFont(unsigned char* path, font_t* font)
{
    char* nw_path = malloc(strlen((const char *)path) + 1);
    strcpy(nw_path, (const char *)path);
    PC_Font* ft = (PC_Font*)malloc(sizeof(PC_Font));
    ft->font = font;
    return PC_AddResource((unsigned char *)nw_path, ft, PC_FONT, &__pc_loadfile, &__pc_freeFont);
}

//*<RSC>*/
VirtualFile* __pc_static_files[1];
int __pc_static_files_count = 0;
//*</RSC>*/

static void __pc_SetupResources()
{
    //*<RSC_LOAD>*/
//*</RSC_LOAD>*/
}

static void __pc_QuitResources()
{
    for (int i = 0; i < __pc_static_files_count; i++)
    {
        __pc_static_files[i]->free(__pc_static_files[i]);
        __pc_static_files[i]->loaded = false;
    }
    //*<RSC_UNLOAD>*/
//*</RSC_UNLOAD>*/
}

void (*__pc_afterInitAsset)() = __pc_SetupResources;
void (*__pc_beforeQuitAsset)() = __pc_QuitResources;