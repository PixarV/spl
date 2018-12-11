#include <stdio.h>
#include <stdlib.h>


int main() {
	int* m = malloc(sizeof(int)*4);

	for (int i = 0; i < 4; i++) {
			printf("%d ", m[i]);
	}

	realloc(m, sizeof(int)*8);

	for (int i = 0; i < 8; i++) {
			printf("%d ", m[i]);
	}

}
