#include "image.h"
#include "common-sce.h"



int __psp_GetPixel_CG_RGB16(image_t* image, int x, int y){
    void *data = image->data + y * image->stride;
    uint16_t *data_u16 = data;
    return data_u16[x];
}

int __psp_GetPixel_CG_P8(image_t* image, int x, int y){
    void *data = image->data + y * image->stride;
    uint8_t *data_u8 = data;
    return (char)data_u8[x];
}

int __psp_GetPixel_CG_P4(image_t* image, int x, int y){
    void *data = image->data + y * image->stride;
    uint8_t *data_u8 = data;
    return (x & 1) ? data_u8[x >> 1] & 0x0f : data_u8[x >> 1] >> 4;
}

int __psp_DecodePixel_CG_RGB16(image_t* image, int pixel){
    (void)image;
    return pixel;
}

int __psp_DecodePixel_CG_P8(image_t* image, int pixel){
    return image->palette[pixel+128];
}

int __psp_DecodePixel_CG_P4(image_t* image, int pixel){
    return image->palette[pixel];
}

void __psp_SetPixel_CG_RGB16(image_t* image, int x, int y, int color){
    void *data = image->data + y * image->stride;
    uint16_t *data_u16 = data;
    data_u16[x] = color;
}

void __psp_SetPixel_CG_P8(image_t* image, int x, int y, int color){
    void *data = image->data + y * image->stride;
    uint8_t *data_u8 = data;
    data_u8[x] = color;
}

void __psp_SetPixel_CG_P4(image_t* image, int x, int y, int color){
    void *data = image->data + y * image->stride;
    uint8_t *data_u8 = data;
    if (x & 1) {
        data_u8[x >> 1] = (data_u8[x >> 1] & 0xf0) | (color & 0x0f);
    } else {
        data_u8[x >> 1] = (data_u8[x >> 1] & 0x0f) | (color << 4);
    }
}


void __psp_SetupFormatImage(image_t* image){
    if(IMAGE_IS_RGB16(image->format)) {
        image->GetPixel =__psp_GetPixel_CG_RGB16;
        image->DecodePixel =__psp_DecodePixel_CG_RGB16;
        image->SetPixel =__psp_SetPixel_CG_RGB16;
    }
    else if(IMAGE_IS_P8(image->format)) {
        image->GetPixel =__psp_GetPixel_CG_P8;
        image->DecodePixel =__psp_DecodePixel_CG_P8;
        image->SetPixel =__psp_SetPixel_CG_P8;
    }
    else if(IMAGE_IS_P4(image->format)) {
        image->GetPixel =__psp_GetPixel_CG_P4;
        image->DecodePixel =__psp_DecodePixel_CG_P4;
        image->SetPixel =__psp_SetPixel_CG_P4;
    }

    image->_alphaValue = image_alpha(image->format);
}

image_t *image_alloc(int width, int height, int format)
{
    image_t *img = image_create(width, height, format);
    if(!img)
        return NULL;
    if(IMAGE_IS_RGB16(format))
        img->stride = ((width + 1) >> 1) * 4;
    else if(IMAGE_IS_P8(format))
        img->stride = width;
    else if(IMAGE_IS_P4(format))
        img->stride = ((width + 1) >> 1);

    void *data = malloc(height * img->stride);
    if(!data) {
        image_free(img);
        return NULL;
    }

    img->data = data;
    img->flags |= IMAGE_FLAGS_DATA_ALLOC;
    __psp_SetupFormatImage(img);
    return img;
}

image_t *image_load(FILE *file)
{
    if(!file)
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
    image_t* img = image_alloc(width,height,format);
    if (img == NULL)
        return NULL;
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
    __psp_SetupFormatImage(img);
    return img;
}

void image_free(image_t *img)
{

    if(img->flags & IMAGE_FLAGS_DATA_ALLOC)
        free(img->data);
    if(img->flags & IMAGE_FLAGS_PALETTE_ALLOC)
        free(img->palette);

    free(img);
}

bool image_alloc_palette(image_t *img, int size)
{
    if(!img || !IMAGE_IS_INDEXED(img->format))
        return false;
    if(img->flags & IMAGE_FLAGS_PALETTE_ALLOC)
        free(img->palette);

    if(IMAGE_IS_P8(img->format)) {
        size = (size <= 0) ? 256 : min(size, 256);
    }
    if(IMAGE_IS_P4(img->format)) {
        size = 16;
    }

    img->palette = calloc(size, 2);
    img->color_count = 0;
    img->flags &= ~IMAGE_FLAGS_PALETTE_ALLOC;

    if(!img->palette)
        return false;

    memset(img->palette, 0, 2*size);
    img->color_count = size;
    img->flags |= IMAGE_FLAGS_PALETTE_ALLOC;
    return true;
}

int image_alpha(int format)
{
    switch(format) {
    case IMAGE_RGB565A:
        return 0x0001;
    case IMAGE_P8_RGB565A:
        return -128;
    case IMAGE_P4_RGB565A:
        return 0;
    default:
        /* A value that cannot be found in any pixel of any format */
        return 0x10000;
    }
}


bool image_copy_palette(image_t const *src, image_t *dst, int size)
{
    if(!image_valid(src) || !dst)
        return false;
    if(!IMAGE_IS_INDEXED(dst->format))
        return true;

    if(size < 0)
        size = src->color_count;
    if(!image_alloc_palette(dst, size))
        return false;

    int N = min(src->color_count, dst->color_count);
    memcpy(dst->palette, src->palette, 2*N);
    return true;
}

image_t *image_create(int width, int height, int format)
{
    if(!IMAGE_IS_RGB16(format) && !IMAGE_IS_P8(format) && !IMAGE_IS_P4(format))
        return NULL;
    if(width <= 0 || width > 0xffff || height <= 0 || height > 0xffff)
        return NULL;

    image_t *img = malloc(sizeof *img);
    if(!img)
        return NULL;

    img->format = format;
    img->flags = 0;
    img->color_count = 0;
    img->width = width;
    img->height = height;
    img->stride = 0;
    img->data = NULL;
    img->palette = NULL;
    img->_alphaValue = 0;
    return img;
}


/*image_t *image_create_vram(void)
{
    image_t *img = image_create(DWIDTH, DHEIGHT, IMAGE_RGB565);
    if(!img)
        return NULL;

    img->stride = 2 * DWIDTH;
    img->data = gint_vram;
    return img;
}*/

void image_set_palette(image_t *img, uint16_t *palette, int size, bool owns)
{
    if(!img || !IMAGE_IS_INDEXED(img->format))
        return;
    if(img->flags & IMAGE_FLAGS_PALETTE_ALLOC)
        free(img->palette);

    img->palette = palette;
    img->color_count = size;

    if(owns)
        img->flags |= IMAGE_FLAGS_PALETTE_ALLOC;
    else
        img->flags &= ~IMAGE_FLAGS_PALETTE_ALLOC;
}

bool image_valid(image_t const *img)
{
    if(!img)
        return false;

    if(IMAGE_IS_RGB16(img->format)) {
        return (img->data != NULL);
    }
    if(IMAGE_IS_P8(img->format) || IMAGE_IS_P4(img->format)) {
        return (img->data != NULL) && (img->palette != NULL) &&
            (img->color_count != 0);
    }

    /* Invalid format */
    return false;
}
