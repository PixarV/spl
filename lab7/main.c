#include "bitmap.h"

extern int sepia_asm_inplace(pixel* img, size_t size, pixel* padding);
//extern int fun(int* ptr);

int main() {
	char* filename = "bmp/sources.bmp";
	image* bmp = malloc(sizeof(image));
	read_status r_status = get_image(bmp, filename);

	//sepia_c_inplace(bmp);

	if (r_status != READ_OK) {
		puts("Nu vot\n");
		return -1;
	}

	sepia_asm_inplace(bmp->img, bmp->width*bmp->height, bmp->img);
//	rotate_on_90(bmp, clockwise);
	rotate_on_180(bmp);

	write_status w_status = write_image(bmp, "hhh.bmp");

	if (w_status != READ_OK) {
		puts("Nu vot\n");
		return -1;
	}

	free(bmp);
}
