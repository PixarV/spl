#include <stdio.h>

const int g_arr1[] = {3, 4, 5, 42};
const int g_arr2[] = {6, 8, 3, 44};

int is_prime(int n) {
    
    return n%2;
}

int main(int argc, char** argv) {
    size_t count = sizeof(g_arr1) / sizeof(int);

    //printf("%d%n", compute_scalar(g_arr1, g_arr2, count));

    printf("%d\n", is_prime(4));

    return 0;
}
