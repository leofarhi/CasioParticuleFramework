#include "ParticuleEngineOverwrited.h"


bopti_image_t* __cg_LoadExternalImage(char* path)
{
    FILE* file = fopen((const char*)path, "rb");
    if (file == NULL)
        return NULL;
    uint8_t format;
    int16_t color_count;
    uint16_t width,height;
    int stride;
    fread(&format,sizeof(uint8_t),1,file);
    fread(&color_count,sizeof(int16_t),1,file);
    fread(&width,sizeof(uint16_t),1,file);
    fread(&height,sizeof(uint16_t),1,file);
    fread(&stride,sizeof(int),1,file);
    bopti_image_t* img = image_alloc(width,height,format);
    if (img == NULL)
    {
        fclose(file);
        return NULL;
    }
    //img->stride = stride;
    img->color_count = color_count;
    img->palette = 0;
    unsigned long long SizeOfData = 0;
    fread(&SizeOfData,sizeof(unsigned long long),1,file);
    fread(img->data,1,SizeOfData,file);
    unsigned int SizeOfPalette = 0;
    fread(&SizeOfPalette,sizeof(unsigned int),1,file);
    if (SizeOfPalette!=0)
    {
        image_alloc_palette(img,color_count);
        fread(img->palette,1,SizeOfPalette,file);
    }
    fclose(file);
    return img;
}

bopti_image_t* __cg_LoadImageSprite(char* path,int index)
{
    FILE* file = fopen((const char*)path, "rb");
    if (file == NULL)
    {
        printf("Error: can't open file %s\n", path);
        return NULL;
    }
    while (index--)
    {
        fseek(file, sizeof(uint8_t)+sizeof(int16_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(int), SEEK_CUR);
        unsigned long long SizeOfData = 0;
        fread(&SizeOfData,sizeof(unsigned long long),1,file);
        fseek(file, SizeOfData, SEEK_CUR);
        unsigned int SizeOfPalette = 0;
        fread(&SizeOfPalette,sizeof(unsigned int),1,file);
        if (SizeOfPalette!=0)
        {
            fseek(file, SizeOfPalette, SEEK_CUR);
        }
    }
    
    uint8_t format;
    int16_t color_count;
    uint16_t width,height;
    int stride;
    fread(&format,sizeof(uint8_t),1,file);
    fread(&color_count,sizeof(int16_t),1,file);
    fread(&width,sizeof(uint16_t),1,file);
    fread(&height,sizeof(uint16_t),1,file);
    fread(&stride,sizeof(int),1,file);
    bopti_image_t* img = image_alloc(width,height,format);
    //img->stride = stride;
    img->color_count = color_count;
    img->palette = 0;
    unsigned long long SizeOfData = 0;
    fread(&SizeOfData,sizeof(unsigned long long),1,file);
    fread(img->data,1,SizeOfData,file);
    unsigned int SizeOfPalette = 0;
    fread(&SizeOfPalette,sizeof(unsigned int),1,file);
    if (SizeOfPalette!=0)
    {
        image_alloc_palette(img,color_count);
        fread(img->palette,1,SizeOfPalette,file);
    }
    fclose(file);
    return img;
}

bopti_image_t* __cg_s__LoadImageSprite(Tuple* args)
{
    char* path = (char*)Tuple_get(args,0);
    int index = (int)Tuple_get(args,1);
    return __cg_LoadImageSprite(path,index);
}

int __cg_GetPixel_CG_RGB16(PC_Texture* texture, int x, int y){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint16_t *data_u16 = data;
    return data_u16[x];
}

int __cg_GetPixel_CG_P8(PC_Texture* texture, int x, int y){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint8_t *data_u8 = data;
    return (int8_t)data_u8[x];
}

int __cg_GetPixel_CG_P4(PC_Texture* texture, int x, int y){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint8_t *data_u8 = data;
    return (x & 1) ? data_u8[x >> 1] & 0x0f : data_u8[x >> 1] >> 4;
}

int __cg_DecodePixel_CG_RGB16(PC_Texture* texture, int pixel){
    (void)texture;
    return pixel;
}

int __cg_DecodePixel_CG_P8(PC_Texture* texture, int pixel){
    return texture->texture->palette[pixel+128];
}

int __cg_DecodePixel_CG_P4(PC_Texture* texture, int pixel){
    return texture->texture->palette[pixel];
}

void __cg_SetPixel_CG_RGB16(PC_Texture* texture, int x, int y, int color){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint16_t *data_u16 = data;
    data_u16[x] = color;
}

void __cg_SetPixel_CG_P8(PC_Texture* texture, int x, int y, int color){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint8_t *data_u8 = data;
    data_u8[x] = color;
}

void __cg_SetPixel_CG_P4(PC_Texture* texture, int x, int y, int color){
    void *data = texture->texture->data + y * texture->texture->stride;
    uint8_t *data_u8 = data;
    if (x & 1) {
        data_u8[x >> 1] = (data_u8[x >> 1] & 0xf0) | (color & 0x0f);
    } else {
        data_u8[x >> 1] = (data_u8[x >> 1] & 0x0f) | (color << 4);
    }
}


void __cg_SetupFormatTexture(PC_Texture* tex){
    bopti_image_t* texture = tex->texture;
    if(IMAGE_IS_RGB16(texture->format)) {
        tex->GetPixel =__cg_GetPixel_CG_RGB16;
        tex->DecodePixel =__cg_DecodePixel_CG_RGB16;
        tex->SetPixel =__cg_SetPixel_CG_RGB16;
    }
    else if(IMAGE_IS_P8(texture->format)) {
        tex->GetPixel =__cg_GetPixel_CG_P8;
        tex->DecodePixel =__cg_DecodePixel_CG_P8;
        tex->SetPixel =__cg_SetPixel_CG_P8;
    }
    else if(IMAGE_IS_P4(texture->format)) {
        tex->GetPixel =__cg_GetPixel_CG_P4;
        tex->DecodePixel =__cg_DecodePixel_CG_P4;
        tex->SetPixel =__cg_SetPixel_CG_P4;
    }

    tex->_alphaValue = image_alpha(texture->format);
}

PC_Color __cg_ToRGB(int color){
    PC_Color c;
    c.r = ((color >> 11) & 0b11111) << 3;
    c.g = ((color >> 5) & 0b111111) << 2;
    c.b = (color & 0b11111) << 3;
    c.a = 255;
    return c;
}

PC_Color __cg_ColorCreateFromInt(PC_Texture* texture, int pixel){
    PC_Color color;
    color.color = pixel;
    if (pixel == texture->_alphaValue)
        color.a = 0;
    else
    {
        color = __cg_ToRGB(pixel);
    }
    return color;
}



int GetWidth(PC_Texture* texture)
{
    return texture->texture->width;
}

int GetHeight(PC_Texture* texture)
{
    return texture->texture->height;
}

PC_Texture* PC_LoadTexture(const char* path)
{
    VirtualFile* file = PC_LoadResource((unsigned char*)path);
    if (file == NULL)
    {
        char str[150];
        sprintf(str, "Error: can't open file %s", path);
        PC_ErrorScreen(str);
    }
    PC_Texture* texture = (PC_Texture*)file->data;
    if (texture->IsExternalPath!=NULL && texture->texture == NULL)
    {
        texture->texture = (bopti_image_t*)gint_world_switch(GINT_CALL(__cg_LoadExternalImage,(void*)texture->IsExternalPath));
        if (texture->texture == NULL)
        {
            char str[150];
            sprintf(str, "Error: can't open file %s", texture->IsExternalPath);
            PC_ErrorScreen(str);
        }
    }
    texture->isAllocated = false;
    __cg_SetupFormatTexture(texture);
    return texture;
}

PC_Texture* PC_CreateTexture(int width, int height)
{
    PC_Texture* texture = malloc(sizeof(PC_Texture));
    texture->texture = image_alloc(width, height, IMAGE_RGB565A);
    texture->IsExternalPath = NULL;
    texture->isAllocated = true;
    __cg_SetupFormatTexture(texture);
    return texture;
}

void PC_FreeTexture(PC_Texture* texture)
{
    if (texture->IsExternalPath!=NULL)
    {
        if (texture->texture != NULL)
            image_free(texture->texture);
        texture->texture = NULL;
    }
    if (texture->isAllocated)
    {
        if (texture->IsExternalPath != NULL)
            free(texture->IsExternalPath);
        texture->IsExternalPath = NULL;
        free(texture);
    }
}

void PC_DrawTexture(PC_Texture* texture, int x, int y)
{
    dimage(x, y, texture->texture);
}

void PC_DrawSubTexture(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh)
{
    dsubimage(x, y,texture->texture, sx, sy, sw, sh, DIMAGE_NONE);
}

void PC_DrawSubTextureSize(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h)
{
    if (sw == w && sh == h)
    {
        PC_DrawSubTexture(texture, x, y, sx, sy, sw, sh);
        return;
    }
    int flipX = 0;
    int flipY = 0;
    
    if (w < 0) {
        flipX = 1;
        w = -w;
    }
    if (h < 0) {
        flipY = 1;
        h = -h;
    }
    int x2 = 0, y2 = 0;
    unsigned int sx2 = 0, sy2 = 0;
    int xinc = (sw << 16) / w;
    int yinc = (sh << 16) / h;
    //limites les bornes de la texture
    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;
    if (sx + sw > texture->texture->width && sw > 0)
    {
        long long ratio = (long long)w * 0x10000 / sw;
        sw = texture->texture->width - sx;
        w = sw * ratio / 0x10000;
    }
    if (sy + sh > texture->texture->height && sh > 0)
    {
        long long ratio = (long long)h * 0x10000 / sh;
        sh = texture->texture->height - sy;
        h = sh * ratio / 0x10000;
    }
    //limites les bornes en fonction de la taille de la fenetre
    if(x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if(x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if(y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    const int alph = texture->_alphaValue;
    int row = 0;
    int col = 0;
    while (row < h) {
        x2 = 0;
        col = 0;
        
        sy2 = sy + (flipY ? (sh - 1 - (y2 >> 16)) : (y2 >> 16));
        while (col < w) {
            sx2 = sx + (flipX ? (sw - 1 - (x2 >> 16)) : (x2 >> 16));
            const int dataPix = texture->GetPixel(texture, sx2, sy2);
            
            if (dataPix != alph)
            {
                gint_vram[DWIDTH * (y + row) + (x + col)] = texture->DecodePixel(texture, dataPix);
            }
            
            x2 += xinc;
            col++;
        }
        
        y2 += yinc;
        row++;
    }
}

void PC_DrawSubTextureSizeColored(PC_Texture* texture, int x, int y, int sx, int sy, int sw, int sh, int w, int h, PC_Color color)
{
    int flipX = 0;
    int flipY = 0;
    
    if (w < 0) {
        flipX = 1;
        w = -w;
    }
    if (h < 0) {
        flipY = 1;
        h = -h;
    }
    int x2 = 0, y2 = 0;
    unsigned int sx2 = 0, sy2 = 0;
    int xinc = (sw << 16) / w;
    int yinc = (sh << 16) / h;
    //limites les bornes de la texture
    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;
    if (sx + sw > texture->texture->width && sw > 0)
    {
        long long ratio = (long long)w * 0x10000 / sw;
        sw = texture->texture->width - sx;
        w = sw * ratio / 0x10000;
    }
    if (sy + sh > texture->texture->height && sh > 0)
    {
        long long ratio = (long long)h * 0x10000 / sh;
        sh = texture->texture->height - sy;
        h = sh * ratio / 0x10000;
    }
    //limites les bornes en fonction de la taille de la fenetre
    if(x < 0) {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0) {
        sy -= y;
        h += y;
        y = 0;
    }
    if(x + w > SCREEN_WIDTH) w = SCREEN_WIDTH - x;
    if(y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
    const int alph = texture->_alphaValue;
    int row = 0;
    int col = 0;
    while (row < h) {
        x2 = 0;
        col = 0;
        
        sy2 = sy + (flipY ? (sh - 1 - (y2 >> 16)) : (y2 >> 16));
        while (col < w) {
            sx2 = sx + (flipX ? (sw - 1 - (x2 >> 16)) : (x2 >> 16));
            const int dataPix = texture->GetPixel(texture, sx2, sy2);
            
            if (dataPix != alph)
            {
                int colPix = texture->DecodePixel(texture, dataPix);
                //multiply color
                PC_Color rgb = __cg_ToRGB(colPix);
                PC_Color colorRes =PC_ColorCreate(rgb.r * color.r / 255, rgb.g * color.g / 255, rgb.b * color.b / 255, 255);
                gint_vram[DWIDTH * (y + row) + (x + col)] = colorRes.color;
            }
            
            x2 += xinc;
            col++;
        }
        
        y2 += yinc;
        row++;
    }
}

void PC_DrawSubTextureRect(PC_Texture* texture, PC_Rect* src, PC_Rect* dest, PC_Color TextureColorMod)
{
    if (dest == NULL)
        return;
    if (PC_ColorEqual(TextureColorMod,PC_WHITE))
    {
        if (src == NULL)
        {
            PC_DrawSubTexture(texture, dest->x, dest->y, GetWidth(texture), GetHeight(texture), dest->w, dest->h);
            return;
        }
        PC_DrawSubTextureSize(texture, dest->x, dest->y, src->x, src->y, src->w, src->h, dest->w, dest->h);
    }
    else
    {
        if (src == NULL)
        {
            PC_DrawSubTextureSizeColored(texture, dest->x, dest->y, 0, 0, GetWidth(texture), GetHeight(texture), dest->w, dest->h, TextureColorMod);
            return;
        }
        PC_DrawSubTextureSizeColored(texture, dest->x, dest->y, src->x, src->y, src->w, src->h, dest->w, dest->h, TextureColorMod);
    }
}

PC_Color PC_GetPixelColor(PC_Texture* texture, int x, int y)
{
    int pixel = texture->GetPixel(texture, x, y);
    return __cg_ColorCreateFromInt(texture, pixel);
}

void PC_SetPixelColor(PC_Texture* texture, int x, int y, PC_Color color)
{
    texture->SetPixel(texture, x, y, color.color);
}

PC_Texture* PC_LoadTextureSprite(const char* path,int index)
{
    VirtualFile* file = PC_LoadResource((unsigned char*)path);
    if (file == NULL)
    {
        char str[150];
        sprintf(str, "Error: can't open file %s", path);
        PC_ErrorScreen(str);
    }
    PC_Texture* texture = (PC_Texture*)file->data;
    texture->texture = NULL;
    texture->isAllocated = false;
    Tuple* args = Tuple_create(2);
    Tuple_set(args,0,(void*)texture->IsExternalPath);
    Tuple_set(args,1,(void*)index);
    texture->texture = (bopti_image_t*)gint_world_switch(GINT_CALL(__cg_s__LoadImageSprite,(void*)args));
    if (texture->texture == NULL)
    {
        char str[150];
        sprintf(str, "Error: can't open file %s", texture->IsExternalPath);
        PC_ErrorScreen(str);
    }
    Tuple_free(args);
    __cg_SetupFormatTexture(texture);
    return texture;
}