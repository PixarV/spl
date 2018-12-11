#include <stdio.h>
#include <math.h>

const int arr1[] = {2, 3, 4, 5};
const int arr2[] = {2, 3, 4, 5};

int compute_scalar(const int* arr1, const int* arr2, const size_t count) {
    int sum = 0;

    for (size_t i = 0; i < count; i++) {
	sum += (arr1[i] * arr2[i]);
    }
    return sum;
}

int is_prime(const unsigned long n) {
    if (n == 0 || n == 1) return 0;
    
    unsigned long div = 2L;
    
    while (div < sqrt(n)) {
	if (!(n % div++)) return 0; 
    }

    return 1; 
}

int main() {
    unsigned long n;
    scanf("%lu", &n);
    printf("%lu", n);

    printf("%s\n", (is_prime(n) == 1) ? "yes" : "no");
    
    size_t count = sizeof(arr1) / sizeof(int);
    printf("%d\n", compute_scalar(arr1, arr2, count));

    return 0;
}
