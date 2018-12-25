#include "bitmap.h"


read_status validate_header(header const* header) {
    if (  (header->bfType != 0x4D42)
       || (header->bfReserved)
       || (header->biSize != 0x28)
       || (header->biWidth < 1 || header->biHeight < 1) 
       || (header->biPlanes != 1))
           return READ_INVALID_BITMAP_HEADER;

    if ( (header->biBitCount != 0x18) || (header->biCompression) )
        return READ_FILE_UNSUPPORTED_VERSION;

	return READ_OK;
}

read_status get_header(header* bmp_header, FILE* file) {
	fread(bmp_header, sizeof(header), 1, file);
	return validate_header(bmp_header);
}

pixel* get_img_data(uint32_t w, uint32_t h, uint16_t d, FILE* file) {
	pixel* img  = malloc(w*h*d);

	int offset = 0;
	for (int i = 0; i < h; i++) {
		int count = w;
		fread(img+offset, sizeof(pixel), count, file);
		offset+=count;
	}

	return img;
}

read_status get_image(image* bmp, char const* filename) {
    FILE* file = fopen(filename, "r");
	if (!file) { return WRITE_INVALID_FILE; }

	header* bmp_header = malloc(sizeof(header));
    
	read_status status = get_header(bmp_header, file);  
	if (status != READ_OK) { return status; }

	bmp->header = bmp_header;

	bmp->width = bmp_header->biWidth;
	bmp->height = bmp_header->biHeight;
	bmp->depth = bmp_header->biBitCount / 8;

	bmp->img = get_img_data(bmp->width, bmp->height, bmp->depth, file);

	fclose(file);
	return READ_OK;
}

write_status write_image(const image* bmp, const char* filename) {
	if (!bmp) { return WRITE_NULL_PTR_IMAGE; }

    FILE* file = fopen(filename, "w");
	if (!file) { return WRITE_INVALID_FILE; }

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
	return WRITE_OK;
}

pixel* counter_clockwise(pixel const* img, uint32_t w, uint32_t h, uint16_t d) {
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

pixel* clockwise(pixel const* img, uint32_t w, uint32_t h, uint16_t d) {
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

	if (w != h) {
		bmp_header->biWidth = h;
		bmp_header->biHeight = w;
		bmp->width = h;
		bmp->height = w;
	}
	
	bmp->img = direction(img, w, h, d);
}

void rotate_on_180(image* bmp) {
	rotate_on_90(bmp, clockwise);
	rotate_on_90(bmp, clockwise);
}

void free_img(image* bmp) {
	free(bmp->img);
	free(bmp->header);
	free(bmp);
}


uint32_t sum(uint32_t a, uint32_t b) {
	return a + b;
}

static unsigned char sat( uint64_t x) {
    if (x < 256) return x; return 255;
}
static void sepia_one( pixel* const pixel_one ) {
    static const float c[3][3] = {
            { .393f, .769f, .189f },
            { .349f, .686f, .168f },
            { .272f, .534f, .131f } };
    pixel old = *pixel_one;
    pixel_one->r = sat(
            old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]
    );
    pixel_one->g = sat(
            old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]
    );
    pixel_one->b = sat(
            old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]
    );
}
void sepia_c_inplace( image* img ) {
    uint32_t x,y;
	pixel* bmp = img->img;
	uint32_t padding = img->width % 4;

    for( y = 0; y < img->height; y++ ) {
        for( x = 0; x < img->width; x++ )
            sepia_one( bmp + x + img->height*y );
		// fix padding
	}
}
