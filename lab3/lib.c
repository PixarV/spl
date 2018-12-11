#include <stddef.h>
#include <stdio.h>
#include "lib.h"


int compute_scalar(const int* arr1, const int* arr2, size_t count) {
    int sum = 0;

    for (size_t i = 0; i < count; i++) {
	sum += (arr1[i] * arr2[i]);
	//puts("dsds");
    }
    return sum;
}

//int is_prime(unsigned long n) {
  //  printf("%d", sizeof(n));
  //  return 0;
//}

