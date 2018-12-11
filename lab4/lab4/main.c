#include "list.h"
#include <stdio.h>

#define forever while(1)

// for each
void foreach_print(int i) {
    printf("FOR EACH FUNC: %d\n", i);
}
void print_spaces(int i) {
    printf("%d ", i);
}
void print_new_lines(int i) {
    printf("%d\n", i);
}

//map 
int inc(int i) {
    return i+2;
}
 
int cubes(int i) {
    return i*i*i;
}
int square(int i) {
    return i*i;
}

int abs(int i) {
    return i > 0 ? i : -i;
}

//foldl
double sum(double accum, int element) {
    return accum + element;
}
double max(double accum, int element) {
    return accum >= element ? accum : element;
}
double min(double accum, int element) {
    return accum <= element ? accum : element;
}

//iterate 

int powers2(int i) {
    return i*2;
}

linked_list* read_list() {
    linked_list* list = NULL;

    int in, result;
    forever {
        result = scanf( "%d", &in );
        if (result == EOF) {
            break;
        } else if (result == 0) {
            printf("Unexpected arument\n");
            return NULL;
        } else {
            if (!list)
                list = list_create(in);
            else
                list_add_front(in, &list);
        }
    } 

    return list;
}

int main() {
    linked_list* list = read_list();
    if (!list) {
        printf("ERROR");
        return 1;
    }

    printf("TEST: list_length()\n");
    int length = list_length(list);
    printf("list_length: %d\n", length);

    linked_list* new_list;
    linked_list* iterate_list;
    double accum;

    #ifdef CUSTOM_TESTS
        printf("TEST: list_print(length+1)\n");
        list_print(length+1, list);
        printf("TEST: list_print(length-1)\n");
        list_print(length-1, list);

        printf("TEST: list_add_front(1)\n");
        list_add_front(1, &list);
        length++;
        printf("print 2 first element: ");
        list_print(2, list);

        printf("TEST: list_add_back(6)\n");
        list_add_back(6, &list);
        length++;
        printf("print elements: ");
        list_print(length, list);

        printf("TEST: list_sum()\n");
        printf("SUM: %d\n", list_sum(list));

        printf("TEST: list_get(2)\n");
        printf("element: %d\n", list_get(2, list));


        printf("TEST: Hight-Order functions\n");
        printf("TEST: foreach(list, print)\n");
        foreach(list, foreach_print);
        printf("TEST: map(list, add(2))\n");
        new_list = map(list, inc);
        foreach(new_list, foreach_print);
        list_free(new_list);
        printf("TEST: map_mut(list, add(2))\n");
        map_mut(list, inc);
        foreach(list, foreach_print);
        printf("TEST: foldl(0, sum(accum, element), list)\n");
        accum = foldl(0, sum, list);
        printf("FOLDL result: %f\n", accum);
        printf("TEST: iterate(3, 5, add(2))\n");
        iterate_list = iterate(3, 5, inc);
        foreach(iterate_list, foreach_print);
        list_free(iterate_list);
    #endif
    
    #ifndef EXCLUDE_ASSIGMENTS
        printf("TEST: ASSIGMENTS\n");
        printf("TEST: foreach(list, print_spaces)\n");
        foreach(list, print_spaces);
        printf("TEST: foreach(list, print_new_lines)\n");
        foreach(list, print_new_lines);

        printf("TEST: map(list, cubes)\n");
        new_list = map(list, cubes);
        foreach(new_list, foreach_print);
        list_free(new_list);
        printf("TEST: map(list, square)\n");
        new_list = map(list, square);
        foreach(new_list, foreach_print);
        list_free(new_list);

        printf("TEST: map_mut(list, abs)\n");
        map_mut(list, abs);
        foreach(list, foreach_print);

        printf("TEST: foldl(0, sum(accum, element), list)\n");
        accum = foldl(0, sum, list);
        printf("FOLDL result: %f\n", accum);
        printf("TEST: foldl(elem, min(accum, element), list)\n");
        accum = foldl(list->element, min, list);
        printf("FOLDL result: %f\n", accum);
        printf("TEST: foldl(elem, max(accum, element), list)\n");
        accum = foldl(list->element, max, list);
        printf("FOLDL result: %f\n", accum);

        printf("TEST: iterate(1, 5, power2)\n");
        iterate_list = iterate(1, 5, powers2);
        foreach(iterate_list, foreach_print);
        list_free(iterate_list);
    #endif
    save(list, "test.txt");
    load(&list, "test.txt");
    foreach(list, foreach_print);

    list_free(list); 
}

