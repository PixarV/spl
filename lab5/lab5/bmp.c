#include "bmp.h"

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <sys/mman.h>

static enum read_status read_bmp_header(FILE *file, bmp_header *header) 
{

    if (fread(&header->file, sizeof(header->file), 1, file) != 1) return READ_INVALID_FILE;

    if (  (header->file.bfType != 0x4D42)
       || (header->file.bfReserved)
       || (fread(&header->info, sizeof(header->info), 1, file) != 1)
       || (header->info.biSize != 0x28)
       || (header->info.biWidth < 1 || header->info.biHeight < 1) 
       || (header->info.biPlanes != 1))
           return READ_INVALID_BITMAP_INFO_HEADER;

    if ( (header->info.biBitCount != 0x18) || (header->info.biCompression) )
        return READ_FILE_UNSUPPORTED_VERSION;

    return READ_OK;
}

pixel *image_get(image *img, uint64_t x, uint64_t y) 
{
    return img->data + y * img->width + x;
}


image image_create( uint64_t width, uint64_t height) 
{
    return (image) {
            .width = width,
            .height = height,
            .data = calloc(PIXEL_SIZE, width * height)
           };
}


void image_destroy(image *img) 
{
    free(img->data);
    free(img);
}

static uint64_t bmp_padding(uint64_t width) { return width % 4; }

enum read_status bmp_from_file(char *file_path, image *const img) 
{
    FILE *file = fopen(file_path, "rb");
    bmp_header header;

    int read_status = read_bmp_header(file, &header);
    if (read_status != READ_OK) return read_status;

    fseek(file, header.file.bfOffBits, SEEK_SET);

    *img = image_create(header.info.biWidth, header.info.biHeight);
    const uint64_t padding = bmp_padding(img->width);

/*
    img->data = mmap(NULL,
    header.file.bfOffBits+(header.info.biWidth*24+padding)*header.info.biHeight,
    PROT_READ,
    MAP_SHARED,
    fileno(file), 0);
    img->data = (struct pixel*)(((char*)img->data) + header.file.bfOffBits);
*/

    for (uint64_t i = 0; i < img->height; i++)
        if (fread(image_get(img, 0, i), img->width * PIXEL_SIZE, 1, file)) {
            fseek(file, bmp_padding(img->width), SEEK_CUR);
        } else {
            image_destroy(img);
            return READ_INVALID_FILE;
        }
    return READ_OK;
}


static bmp_header bmp_header_generate(const image *img) 
{
    bmp_header header;
    header.file = (bmp_file_header) {
            .bfType=0x4D42,
            .bfReserved = 0,
            .bfOffBits = sizeof(bmp_header),
            .bfSize = (uint32_t) (sizeof(bmp_header) + img->width * 3 * img->height)
    };

    header.info = (bmp_info_header) {0};
    header.info.biSizeImage = (uint32_t) ((bmp_padding(img->width) + img->width * 3) * img->height);
    header.info.biSize = 0x28;
    header.info.biWidth = (uint32_t) (img->width);
    header.info.biHeight = (uint32_t) (img->height);
    header.info.biPlanes = 1;
    header.info.biBitCount = 0x18;
    return header;
}


enum write_status bmp_to_file(char *file_path, image *const img) 
{
    FILE* file = fopen(file_path, "wb");

    if (!file) return WRITE_INVALID_FILE;
    if (!img) return WRITE_NULL_PTR_IMAGE;


    bmp_header header = bmp_header_generate(img);
    fwrite(&header, sizeof(header), 1, file);
    const uint64_t padding = bmp_padding(img->width);

    uint64_t stub = 0;

    for (uint64_t i = 0; i < img->height; i++)
        if (!fwrite(image_get(img, 0, i), img->width * PIXEL_SIZE, 1, file)
           || (padding && !fwrite(&stub, padding, 1, file)))
            return WRITE_INVALID_FILE;

    return WRITE_OK;
}


static void swap_pixel(pixel *left, pixel *right) 
{
    pixel *temp = malloc(PIXEL_SIZE);
    memcpy(temp, left, PIXEL_SIZE);
    memcpy(left, right, PIXEL_SIZE);
    memcpy(right, temp, PIXEL_SIZE);
    free(temp);
}

int rotate180(image *img) 
{
    if (!img)
        return 0;
    for (uint64_t i = 0; i < img->height / 2; ++i)
        for (uint64_t j = 0; j < img->width; ++j)
            swap_pixel(image_get(img, i, j),
                       image_get(img, img->height - i - 1, img->width - j - 1));
    return 1;
}


static void swap_squad(pixel *one, pixel *two, pixel *three, pixel *four) 
{
    pixel *temp = malloc(PIXEL_SIZE);

    memcpy(temp, one, PIXEL_SIZE);
    memcpy(one, two, PIXEL_SIZE);
    memcpy(two, three, PIXEL_SIZE);
    memcpy(three, four, PIXEL_SIZE);
    memcpy(four, temp, PIXEL_SIZE);

    free(temp);
}

int rotate90(image *img) 
{
    if (!img)
        return 0;
    for (uint64_t i = 0; i < img->height / 2; ++i)
        for (uint64_t j = 0; j < img->width / 2; ++j) {
            pixel* one = image_get(img, i, j);
            pixel* two = image_get(img, j, img->height - i - 1);
            pixel* three = image_get(img, img->height - i - 1, img->width - j - 1);
            pixel* four = image_get(img, img->width - j - 1, i);
            swap_squad(one, two, three, four);
        }
    return 1;
}

