#include "list.h"

#ifndef COUNT
 #define COUNT 10
#endif

int main() {
    linked_list* list = list_create(0);
    for (int i = 1; i< COUNT; i++)
        list_add_front(i, &list);
}
