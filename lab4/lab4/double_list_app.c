#include <malloc.h>
typedef struct double_list {
    int element;
    struct double_list* next;
    struct double_list* prev;
} double_list;

double_list * create(int val) {
    double_list * list = malloc(sizeof(double_list));
    list->element = val;
    list->next = 0;
    list->prev = 0;
    return list;
}

void append(int val, double_list** list) {
    double_list* new_list = create(val);
    new_list->prev = *list;
    (*list)->next = new_list;
    *list = new_list;
}

#ifndef COUNT
 #define COUNT 10
#endif

int main() {
    double_list* list = create(0);

    for(int i = 0; i < COUNT; i++)
        append(i, &list);
}
