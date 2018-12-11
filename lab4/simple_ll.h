#ifndef _SIMPLE_LL_H_
#define _SIMPLE_LL_H_

#include <stdbool.h>

#include "ll_structs.h"

typedef void(consumer)(const int);
typedef int(function)(const int);
typedef int(bi_function)(const int, const int);

struct list* list_create_empty();
struct list* list_create(const int data);	

void list_add_back(struct list* list, const int data); 
void list_add_front(struct list* list, const int data); 
int list_get(const struct list* list, const int index); 
void list_free(struct list* list);
int list_length(const struct list* list);
struct node* list_node_at(const struct list* list, const int index); 
int list_sum(const struct list* list); 
void print_list(const struct list* list);

// second part
void foreach(const struct list* list, consumer* action);
struct list* map(const struct list* list, function* mapper); 
int foldl(const struct list* list, int identity, bi_function* accumulator);
void map_mut(const struct list* list, function* mapper);
struct list* iterate(int identity, int const length, function* f);

bool save(struct list* list, const char* filename);
bool load(struct list** list, const char* filename);
bool serialize(struct list* list, const char* filename);
bool deserialize(struct list** list, const char* filename);

#endif
