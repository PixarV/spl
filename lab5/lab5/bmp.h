#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>
#include <stdio.h>


typedef struct __attribute__((__packed__)) {
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;
} bmp_file_header;


typedef struct __attribute__((__packed__)) {
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
} bmp_info_header;


typedef struct bmp_header {
    bmp_file_header file;
    bmp_info_header info;
} bmp_header;


typedef struct pixel {
    uint8_t r, g, b;
} pixel;

#define PIXEL_SIZE sizeof(pixel)


typedef struct {
    uint64_t width, height;
    pixel *data;
} image;


enum read_status {
    READ_OK = 0,
    READ_INVALID_BITMAP_FILE_HEADER,
    READ_INVALID_BITMAP_INFO_HEADER,
    READ_INVALID_FILE,
    READ_FILE_UNSUPPORTED_VERSION,
    READ_NULL_PTR_FILE
};

enum write_status {
    WRITE_OK = 0,
    WRITE_INVALID_FILE,
    WRITE_NULL_PTR_IMAGE
};

pixel *image_get(image *img, uint64_t x, uint64_t y);
image image_create(uint64_t width, uint64_t height);

enum read_status bmp_from_file(char *file_path, image *const img);
enum write_status bmp_to_file(char *file_path, image *const img);

int rotate90(image * img);
int rotate180(image * img);

void image_destroy(image *img);

#endif

