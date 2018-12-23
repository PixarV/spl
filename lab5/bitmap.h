#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static const uint8_t zero = 0;

typedef struct __attribute__((__packed__)) {
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;

    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} header;

typedef struct __attribute__((__packed__)) {
    uint8_t r, g, b;
} pixel;

typedef struct image {
	header* header;
    uint32_t width, height;
    uint16_t depth;
    pixel* img;
} image;

typedef pixel*(direction)(const pixel*, uint32_t, uint32_t, uint16_t);

typedef enum read_status {
    READ_OK = 0,
    READ_INVALID_BITMAP_HEADER,
    READ_INVALID_FILE,
    READ_FILE_UNSUPPORTED_VERSION,
    READ_NULL_PTR_FILE
} read_status;

typedef enum write_status {
    WRITE_OK = 0,
    WRITE_INVALID_FILE,
    WRITE_NULL_PTR_IMAGE
} write_status;

void rotate_on_90(image* bmp, direction* direction);
void rotate_on_180(image* bmp);
read_status get_image(image* bmp, char const* filename);
write_status write_image(image const* image, char const* filename);
pixel* clockwise(const pixel* img, uint32_t w, uint32_t h, uint16_t d);
pixel* counter_clockwise(const pixel* img, uint32_t w, uint32_t h, uint16_t d);
void free_img(image* bmp);

#endif
