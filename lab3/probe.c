#include <stdio.h>

int main() {
    char* str = "ritt";
    //str = "hey";
    //*str[2] = 0x72;
    printf("%c\n", str[2]);
    int a[] = {1, 2, 3};

    int* b = a;
    printf("%ld\n", 2L<<31);
    printf("%d", 2[a]);
}
