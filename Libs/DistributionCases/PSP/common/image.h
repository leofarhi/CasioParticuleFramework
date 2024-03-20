#ifndef PSP_IMAGE_H
#define PSP_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
    IMAGE_RGB565      = 0,  /* RGB565 without alpha */
    IMAGE_RGB565A     = 1,  /* RGB565 with one transparent color */
    IMAGE_P8_RGB565   = 4,  /* 8-bit palette, all opaque colors */
    IMAGE_P8_RGB565A  = 5,  /* 8-bit with one transparent color */
    IMAGE_P4_RGB565   = 6,  /* 4-bit palette, all opaque colors */
    IMAGE_P4_RGB565A  = 3,  /* 4-bit with one transparent color */

    IMAGE_DEPRECATED_P8 = 2,
};

/* Quick macros to compare formats by storage size */
#define IMAGE_IS_RGB16(format) \
    ((format) == IMAGE_RGB565 || (format) == IMAGE_RGB565A)
#define IMAGE_IS_P8(format) \
    ((format) == IMAGE_P8_RGB565 || (format) == IMAGE_P8_RGB565A)
#define IMAGE_IS_P4(format) \
    ((format) == IMAGE_P4_RGB565 || (format) == IMAGE_P4_RGB565A)
/* Check whether image format has an alpha color */
#define IMAGE_IS_ALPHA(format) \
    ((format) == IMAGE_RGB565A || \
     (format) == IMAGE_P8_RGB565A || \
     (format) == IMAGE_P4_RGB565A)
/* Check whether image format uses a palette */
#define IMAGE_IS_INDEXED(format) \
    (IMAGE_IS_P8(format) || IMAGE_IS_P4(format))

/* Image flags. These are used for memory management, mostly. */
enum {
    IMAGE_FLAGS_DATA_RO         = 0x01,  /* Data is read-only */
    IMAGE_FLAGS_PALETTE_RO      = 0x02,  /* Palette is read-only */
    IMAGE_FLAGS_DATA_ALLOC      = 0x04,  /* Data is malloc()'d */
    IMAGE_FLAGS_PALETTE_ALLOC   = 0x08,  /* Palette is malloc()'d */
};


#ifndef unit8_t
typedef unsigned char uint8_t;
#endif
#ifndef int16_t
typedef short int16_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif


typedef struct image_t image_t;
typedef struct image_t
{
    /* Color format, one of the IMAGE_* values defined above */
    uint8_t format;
    /* Additional flags, a combination of IMAGE_FLAGS_* values */
    uint8_t flags;
    /* Number of colors in the palette; this includes alpha for transparent
       images, as alpha is always the first entry.
       RGB16: 0
       P8: Ranges between 1 and 256
       P4: 16 */
    int16_t color_count;
    /* Full width and height, in pixels */
    uint16_t width;
    uint16_t height;
    /* Byte stride between lines */
    int stride;

    /* Pixel data in row-major order, left to right.
       - RGB16: 2 bytes per entry, each row padded to 4 bytes for alignment.
         Each 2-byte value is an RGB565 color.
       - P8: 1 signed byte per entry. Each byte is a palette index shifted by
         128 (to access the color, use palette[<value>+128]).
       - P4: 4 bits per entry, each row padded to a full byte. Each entry is a
         direct palette index between 0 and 15. */
    void *data;

    /* For P8 and P4, color palette. The number of entries allocated in the
       array is equal to the color_count attribute. */
    uint16_t *palette;

    int (*GetPixel)(image_t* image, int x, int y);
    int (*DecodePixel)(image_t* image, int pixel);
    void (*SetPixel)(image_t* image, int x, int y, int color);
    int _alphaValue;
} image_t;

/* image_alloc(): Create a new (uninitialized) image

   This function allocates a new image of the specified dimensions and format.
   It always allocates a new data array; if you need to reuse a data array, use
   the lower-level image_create() or image_create_sub().

   The first parameters [width] and [height] specify the dimensions of the new
   image in pixels. The [format] should be one of the IMAGE_* formats, for
   example IMAGE_RGB565A or IMAGE_P4_RGB565.

   This function does not specify or initialize the palette of the new image;
   use image_set_palette(), image_alloc_palette() or image_copy_palette()
   after calling this function.

   The returned image structure must be freed with image_free() after use.

   @width         Width of the new image
   @height        Height of the new image
   @format        Pixel format; one of the IMAGE_* formats defined above */
image_t *image_alloc(int width, int height, int format);

image_t *image_load(FILE *file);

/* image_set_palette(): Specify an external palette for an image

   This function sets the image's palette to the provided address. The number
   of entries allocated must be specified in size. It is also the caller's
   responsibility to ensure that the palette covers all the indices used in the
   image data.

   The old palette, if owned by the image, is freed. If [owns=true] the
   palette's ownership is given to the image, otherwise it is kept external. */
void image_set_palette(image_t *img, uint16_t *palette, int size, bool owns);

/* image_alloc_palette(): Allocate a new palette for an image

   This function allocates a new palette for an image. The number of entries is
   specified in size; for P8 it can vary between 1 and 256, for P4 it is
   ignored (P4 images always have 16 colors).

   The old palette, if owned by the image, is freed. The entries of the new
   palette are all initialized to 0. If size is -1, the format's default
   palette size is used. Returns true on success. */
bool image_alloc_palette(image_t *img, int size);

/* image_copy_palette(): Copy another image's palette

   This function allocates a new palette for an image, and initializes it with
   a copy of another image's palette. For P8 the palette can be resized by
   specifying a value other than -1 as the size; by default, the source image's
   palette size is used (within the limits of the new format). Retuns true on
   success. */
bool image_copy_palette(image_t const *src, image_t *dst, int size);

/* image_create(): Create a bare image with no data/palette

   This function allocates a new image structure but without data or palette.
   The [data] and [palette] members are NULL, [color_count] and [stride] are 0.

   This function is useful to create images that reuse externally-provided
   information. It is intended that the user of this function sets the [data]
   and [stride] fields themselves, along with the IMAGE_FLAGS_DATA_ALLOC flag
   if the image should own its data.

   The [palette] and [color_count] members can be set with image_set_palette(),
   image_alloc_palette(), image_copy_palette(), or manually.

   The returned image structure must be freed with image_free() after use. */
image_t *image_create(int width, int height, int format);

/* image_create_vram(): Create a reference to gint_vram

   This function creates a new RGB565 image that references gint_vram. Using
   this image as target for transformation functions can effectively render
   transformed images to VRAM.

   The value of gint_vram is captured when this function is called, and does
   not update after dupdate() when triple-buffering is used. The user should
   account for this option. (Using this function twice then replacing one of
   the [data] pointers is allowed.)

   The VRAM image owns no data but it does own its own structure so it must
   still be freed with image_free() after use. */
//image_t *image_create_vram(void);

/* image_free(): Free and image and the data it owns

   This function frees the provided image structure and the data that it owns.
   Images converted by fxconv should not be freed; nonetheless, this functions
   distinguishes them and should work. Images are not expected to be created on
   the stack.

   If the image has the IMAGE_FLAGS_DATA_ALLOC flag, the data pointer is also
   freed. Similarly, the image has the IMAGE_FLAGS_PALETTE_ALLOC flag, the
   palette is freed. Make sure to not free images when references to them still
   exist, as this could cause the reference's pointers to become dangling. */
void image_free(image_t *img);

//---
// Basic image access and information
//---

/* image_valid(): Check if an image is valid
   An image is considered valid if it has a valid profile, a non-NULL data
   pointer, and for palette formats a valid palette pointer. */
bool image_valid(image_t const *img);

/* image_alpha(): Get the alpha value for an image format

   This function returns the alpha value for any specific image format:
   * RGB16: 0x0001
   * P8: -128 (0x80)
   * P4: 0
   For non-transparent formats, it returns a value that is different from all
   valid pixel values of the format, which means it is always safe to compare a
   pixel value to the image_alpha() of the format. */
int image_alpha(int format);


#ifdef __cplusplus
}
#endif
#endif // PSP_IMAGE_H