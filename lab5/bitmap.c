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

typedef pixel*(direction)(const pixel*, const uint32_t, const uint32_t, const uint16_t);

header* get_header(FILE* file) {
	header* bmp_header = malloc(sizeof(header));
	fread(bmp_header, sizeof(header), 1, file);
	return bmp_header;
}

pixel* get_img_data(const uint32_t w, const uint32_t h, const uint16_t d, FILE* file) {
	pixel* img  = malloc(w*h*d);

	int offset = 0;
	for (int i = 0; i < h; i++) {
		int count = w;
		fread(img+offset, sizeof(pixel), count, file);
		offset+=count;
	}

	return img;
}

image* get_image(char* filename) {
    FILE* file = fopen(filename, "rb");

	image* bmp = malloc(sizeof(image));
    header* bmp_header = get_header(file);  
	bmp->header = bmp_header;

	bmp->width = bmp_header->biWidth;
	bmp->height = bmp_header->biHeight;
	bmp->depth = bmp_header->biBitCount / 8;

	bmp->img = get_img_data(bmp->width, bmp->height, bmp->depth, file);

	fclose(file);
	return bmp;
}

void write_image(const image* bmp, const char* filename) {
    FILE* file = fopen(filename, "wb");

	fwrite(bmp->header, sizeof(header), 1, file);

	int offset = 0;
	for (int i = 0; i < bmp->height; i++) {
		int padding = bmp->width % 4;
		int count = bmp->width;
		
		fwrite((bmp->img)+offset, sizeof(pixel), count, file);
		fwrite(&zero, 1, padding, file);
		offset+=count;
	}

	fclose(file);
}

static pixel* counter_clockwise(const pixel* img, const uint32_t w, const uint32_t h, const uint16_t d) {
	pixel* temp = malloc(w*h*d);
	int offset = 0;

	for (int i = 0; i < w; i++) {
		for (int j = h-1; j >= 0 ; j--) {
			*(temp+offset) = *(img+i+j*w);
			offset++;
		}
	}

	return temp;
}

static pixel* clockwise(const pixel* img, const uint32_t w, const uint32_t h, const uint16_t d) {
	pixel* temp = malloc(w*h*d);
	int offset = 0;

	for (int i = w-1; i >= 0; i--) {
		for (int j = 0; j < h; j++) {
			*(temp+offset) = *(img+i+j*w);
			offset++;
		}
	}

	return temp;
}

void rotate_on_90(image* bmp, direction* direction) {

	header* bmp_header = bmp->header;
	uint32_t w = bmp->width;
	uint32_t h = bmp->height;
	uint16_t d = bmp->depth;
	pixel* img = bmp->img;
	pixel* temp = malloc(w*h*d);

	if (w != h) {
		bmp_header->biWidth = h;
		bmp_header->biHeight = w;
		bmp->width = h;
		bmp->height = w;
	}
	
	bmp->img = direction(img, w, h, d);
}

int main() {
	char* filename = "bmp/duck.bmp";
		
	
	image* bmp = get_image(filename);

	//rotate_on_90(header, image, counter_clockwise);
	rotate_on_90(bmp, clockwise);
	rotate_on_90(bmp, clockwise);

	write_image(bmp, "hhh.bmp");
}

