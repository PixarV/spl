#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ll_structs.h"
#include "simple_ll.h"

void print_list(const struct list* list) {
	if (!list) { return; }

	struct node* node;
	for(node = list->head; node; node = node->next) {
		printf("%d ", node->data);
	}
}

struct list* list_create_empty() {
	struct list* list = malloc(sizeof(struct list));
	if (!list) { return NULL; }

	list->head = NULL;
	return list;
}

static struct node* initialize_node(const int data) {
	struct node* node = malloc(sizeof(struct node));
	if (!node) { return NULL; }

	node->data = data;
	node->next = NULL;
	return node;	
}

struct list* list_create(const int data) {
	struct node* node = initialize_node(data);
	struct list* list = list_create_empty();
	if (!list || !node) { return NULL; }

	list->head = node;
	return list;
}	

void list_add_back(struct list* list, const int data) {
	struct node* elem = initialize_node(data);
	struct node* node = list->head;
	
	if (!node) {
		list->head = elem;
		return;
	}
   	while (node->next) { node = node->next; }
	node->next = elem;
}

void list_add_front(struct list* list, const int data) {
	struct node* elem = initialize_node(data);

	elem->next = list->head;
	list->head = elem;
}

int list_sum(const struct list* list) {
	int result = 0;
	struct node* node;

	for(node = list->head; node; node = node->next) {
		result += node->data;
	}
	return result;
}

int list_length(const struct list* list) {
	if (!list || !list->head) { return -1; }

	int result = 0;
	struct node* node;

	for(node = list->head; node; node = node->next) {
		result++;
	}
	return result;
}

struct node* list_node_at(const struct list* list, const int index) {
	if (!list || !list->head) { return NULL; }

	int i = 0;
	struct node* node;

	for(node = list->head; node; node = node->next) {
		if (i == index) return node;
		i++;
	}
	return NULL;
}

int list_get(const struct list* list, const int index) {
	if (!list || !list->head) { return 0; }

	struct node* node = list_node_at(list, index);
	return node ? node->data : 0; 
}


void list_free(struct list* list) {
	struct node* node = list->head;

	struct node* temp;
	while(node) {
		temp = node->next;
		free(node);
		node = temp;
	}

	free(list);
}

void foreach(const struct list* list, consumer* action) {
	if (!list || !list->head) { return; }

	struct node* node;
	for (node = list->head; node; node = node->next) {
		action(node->data);
	}
}

struct list* map(const struct list* list, function* mapper) {
	if (!list || !list->head) { return NULL; }
	struct list* result = list_create_empty();

	struct node* node;
	for (node = list->head; node; node = node->next) {
		int temp = mapper(node->data);
		list_add_back(result, temp);
	}
	return result;
}
 
int foldl(const struct list* list, int identity, bi_function* accumulator) {
	struct node* node;
	for (node = list->head; node; node = node->next) {
		identity = accumulator(identity, node->data);
	}
	return identity;
}

void map_mut(const struct list* list, function* mapper) {
	if (!list || !list->head) { return; }

	struct node* node;
	for (node = list->head; node; node = node->next) {
		node->data = mapper(node->data);
	}
	
}

struct list* iterate(int identity, const int length, function* f) {
	struct list* result = list_create(identity);
	for (int i = 0; i < length; i++) {
		identity = f(identity);
		list_add_back(result, identity);
	}
	return result;
}

static bool write(const struct list* list, const char* filename, bool is_binary) {
	 if (!list) { return false; }
    
	 FILE* file = fopen(filename, is_binary ? "wb" : "w");
   	 if (!file) { return false; }

   	 struct node* node;
   	 for (node = list->head; node; node = node->next) {
   	     fprintf(file, "%d ", node->data);
   	 }

   	 fclose(file);
   	 return true;
}

static bool read(struct list** list, const char* filename, bool is_binary) {
	 if (!list) { return false; }
   	 
   	 FILE* file = fopen(filename, is_binary ? "rb" : "r");
   	 if (!file) { return false; }

   	 if (!*list) { *list = list_create_empty(); }
	 
   	 int data;
   	 int eof = fscanf(file, "%d", &data);
   	 while (eof != EOF) {
	     if (eof == 0) { return false; }

   	     list_add_back(*list, data);
   	     eof = fscanf(file, " %d", &data);
   	 }

   	 fclose(file);
   	 return true;
}

bool save(struct list* list, const char* filename) {
    return write(list, filename, false);
}

bool load(struct list** list, const char* filename) {
    return read(list, filename, false);
}

bool serialize(struct list* list, const char* filename) {
    return write(list, filename, 1);
}

bool deserialize(struct list** list, const char* filename) {
    return read(list, filename, 1);
}

