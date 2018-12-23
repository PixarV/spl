#include "bitmap.h"

int main() {
	char* filename = "bmp/duck.bmp";
	image* bmp = malloc(sizeof(image));
	read_status r_status = get_image(bmp, filename);

	if (r_status != READ_OK) {
		puts("Nu vot\n");
		return -1;
	}
	
//	rotate_on_90(bmp, clockwise);
	rotate_on_180(bmp);

	write_status w_status = write_image(bmp, "bmp/duck.bmp");

	if (w_status != READ_OK) {
		puts("Nu vot\n");
		return -1;
	}

	free(bmp);
}
