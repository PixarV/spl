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

double_list* list_node_at(int index, double_list* list) {
    int count = 0;
    double_list* it = 0;
    for (it = list; (it != 0 || count < index); it = it->next, count++);
    if (count == index) {
        return it;
    }
    return 0;
}

int list_get(int index, double_list* list) {
    double_list *node = list_node_at(index, list);
    if (node)
        return node->element;
    else
        return 0;
}

#ifndef COUNT
 #define COUNT 10
#endif

int main() {
    double_list* list = create(0);

    for(int i = 0; i < COUNT; i++)
        append(i, &list);

    for (int i = 0; i< COUNT; i++)
        list_get(i, list);
}
