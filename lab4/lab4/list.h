#ifndef LINKED_LIST
#define LINED_LIST

typedef struct Linked_list {
    int element;
    struct Linked_list* next;
} linked_list;

linked_list* list_create(int const number);
void list_free(linked_list* list);


void list_add_front(int const number, linked_list** list);
void list_add_back(int const number, linked_list** list);

int list_length(linked_list* const list);
int list_sum(linked_list* const list);
int list_print(int const count, linked_list* const list);

int list_get(int index, linked_list* const list);
linked_list* list_node_at(int index, linked_list* const list);

// Hight Ordered functions
typedef void(*for_each_func)(int);
void foreach(linked_list* const list, for_each_func func);

typedef int(*map_func)(int);
linked_list* map(linked_list* const list, map_func func);
void map_mut(linked_list* const list, map_func func);

typedef double(*foldl_func)(double, int);
double foldl(double acc, foldl_func func, linked_list* list);

typedef int(*iterate_func)(int);
linked_list* iterate(int initial_value, int lenght, iterate_func func);

int save(linked_list* lst, const char* filename);
int load(linked_list** lst, const char* filename);
int serialize(linked_list* lst, const char* filename);
int deserialize(linked_list** lst, const char* filename);


int delete_el(int element, linked_list* lst) {
}
#endif
