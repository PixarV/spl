#include "list.h"
#include <malloc.h>
#include <stdio.h>

linked_list* list_create(int const number) {
    linked_list* node = malloc(sizeof(linked_list));
    node->element = number;
    node->next = NULL;
    return node;
}

void list_add_front(int const number, linked_list** list) {
    if (!list)
        return;
    linked_list* node = list_create(number);

    node->next = *list;
    *list = node;
}

void list_add_back(int const number, linked_list** list) {
    if (!list)
        return;
    linked_list* node = list_create(number);
    
    linked_list* it = NULL;
    for(it = *list; it->next != NULL; it = it->next) ;
    it->next = node;
}

int list_length(linked_list* const list) {
    if (!list)
        return -1;
    int length = 1;
    linked_list* node = NULL;
    for(node = list; ; node = node->next, length++) {
        if (!node->next)
            break;
    }
    return length;
}

int list_print(int const count, linked_list* const list) {
    if (!list)
        return -1;
    int index = 0;
    linked_list* node = NULL;
    for(node = list; index < count; node = node->next, index++) {
        printf("%d ", node->element);
        if (!node->next)
            break;
    }
    printf("\n");
    if (index + 1 < count) {
        printf("You ask to write more elements that list have\n");
        return -2;
    }
    return 0;
}

int list_sum(linked_list* const list) {
    if (!list)
        return 0;
    int sum = 0;
    linked_list* node = NULL;
    for(node = list; ; node = node->next) {
        sum += node->element;
        if (!node->next)
            break;
    }
    return sum;
}

linked_list* list_node_at(int index, linked_list* list) {
    int count = 0;
    linked_list* it = NULL;
    for (it = list; (it != NULL || count < index); it = it->next, count++);
    if (count == index) {
        return it;
    }
    return 0;
}

int list_get(int index, linked_list* list) {
    linked_list *node = list_node_at(index, list);
    if (node)
        return node->element;
    else
        return 0;
}

void list_free(linked_list* list) {
    linked_list* it = list;
    while (it != NULL) {
        linked_list* list_for_free = it;
        it = it->next;
        list_for_free->next = NULL;
        free(list_for_free);
    }
}

void foreach(linked_list* const list, for_each_func func) {
    linked_list* it = list;
    while (it != NULL) {
        func(it->element);
        it = it->next;
    }
}

linked_list* map(linked_list* const list, map_func func) {
    linked_list* output_list = NULL;
    linked_list* it = list;

    int new_element;
    while (it != NULL) {
        new_element = func(it->element);
        if (!output_list) {
            output_list = list_create(new_element);
        } else {
            list_add_back(new_element, &output_list);
        }
        it = it->next;
    }
    return output_list;
}

void map_mut(linked_list* const list, map_func func) {
    linked_list* it = list;
    while (it != NULL) {
        it->element = func(it->element);
        it = it->next;
    }
}

double foldl(double accum, foldl_func func, linked_list* list) {
    linked_list* it = list;
    while (it != NULL) {
        accum = func(accum, it->element);
        it = it->next;
    }
    return accum;
}

linked_list* iterate(int initial_value, int length, iterate_func func) {
    linked_list* output_list = NULL;
    int prev_element;
    for (int i = 0; i < length; i++) {
        if (!output_list) {
            output_list = list_create(initial_value);
        } else {
            list_add_front(func(prev_element), &output_list);
        }
        prev_element = output_list->element;
    }

    return output_list;
}

static int file_save(linked_list* list, const char* filename, int binary) {
    char *mode = binary ? "wb" : "w";
    if (!list)
        return 0;
    FILE *file = fopen(filename, mode);
    if (!file)
        return 0;
    linked_list* it = list;
    while (it != NULL) {
        fprintf(file, " %d", it->element);
        it = it->next;
    }
    fclose(file);
    if (ferror(file))
        return 0;
    return 1;
}

static int file_read(linked_list** list, const char* filename, int binary) {
    char *mode = binary ? "rb" : "r";
    if (!list)
        return 0;
    FILE *file = fopen(filename, mode);
    if (!file)
        return 0;

    int element;
    int error = fscanf(file, " %d", &element);
    *list = NULL;

    while (error != EOF && error != 0) {
        printf("Error: %d, Element: %d\n", error, element);
        if (!*list)
            *list = list_create(element);
        else
            list_add_front(element, list);

        error = fscanf(file, " %d", &element);
        if (error == 0) 
            return 0;
    }

    fclose(file);
    if (ferror(file))
        return 0;
    return 1;
}

int save( linked_list* lst, const char* filename) {
    return file_save(lst, filename, 0);
}

int load( linked_list** lst, const char* filename) {
    return file_read(lst, filename, 0);
}

int serialize( linked_list* lst, const char* filename) {
    return file_save(lst, filename, 1);
}
int deserialize( linked_list** lst, const char* filename) {
    return file_read(lst, filename, 1);
}
