#include <malloc.h>

#ifndef COUNT
 #define COUNT 10
#endif

int main() {
    int *array = malloc(sizeof(int) * COUNT);
    for (int i = 0; i< COUNT; i++)
        array[i] = i;
}
