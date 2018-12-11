#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "ll_structs.h"
#include "simple_ll.h"

void print_by_space(const int data) {
	printf("%d ", data);
}

void print_by_new_line(const int data) {
	printf("\n%d", data);
}

int square(const int a) {
	return pow(a, 2);
}

int cube(const int a) {
	return pow(a, 3);
}

int sum(const int a, const int b) {
	return a+b;
}

int min(const int a, const int b) {
	return a < b ? a : b; 
}

int max(const int a, const int b) {
	return a > b ? a : b; 
}

int abs(const int a) {
	return abs(a);
}

int mul_by_2(const int a) {
	return 2 * a;
}

struct list* read_in_reverse_order() {
	struct list* list = NULL;

    int data;
    while(scanf("%d", &data) != EOF) {
        if (!list) {
			list = list_create(data);
		} else {
            list_add_front(list, data);
		}
    } 

    return list;
}


int main() {
	struct list* list_reverse = read_in_reverse_order();
	printf("%d\n", foldl(list_reverse, INT_MAX, min)); 
	printf("%d\n", foldl(list_reverse, INT_MIN, max)); 
	foreach(list_reverse, print_by_space);
	

	struct list* list = list_create_empty();
	list_add_back(list, 7);
	list_add_back(list, 6);
	list_add_back(list, 8);
	list_add_front(list, -1);
	list_add_front(list, -42);

	printf("%d\n", list_sum(list));
	printf("len: %d\n", list_length(list));
	printf("%d\t%d\n", list_node_at(list, 1)->data, list_get(list, 1));
	printf("%d\t%d\n", list_node_at(list, 0)->data, list_get(list, 0));
	printf("%d\t%d\n", list_node_at(list, 3)->data, list_get(list, 3));
	printf("%d\t%d\n", list_node_at(list, 2)->data, list_get(list, 2));
	printf("%d\t%d\n", 5, list_get(list, -2));
	print_list(list);
	puts("\n---------------------");
	
	foreach(list, print_by_space);
	foreach(list, print_by_new_line);
	puts("\n---------------------");
	struct list* temp1 = map(list, square);
	struct list* temp2 = map(list, cube);

	foreach(temp1, print_by_space);
	foreach(temp2, print_by_space);
	puts("\n---------------------");

	printf("%d\n", foldl(list, 0, sum)); 
	printf("%d\n", foldl(list, INT_MAX, min)); 
	printf("%d\n", foldl(list, INT_MIN, max)); 

	puts("\nWith empty list---------------------");
	struct list* empty = list_create_empty();
	foreach(empty, print_by_space);
	printf("%d\n", foldl(empty, INT_MAX, min)); 

	puts("\n---------------------");
	map_mut(list, abs);
	foreach(list, print_by_space);
				
	puts("\n---------------------");
	struct list* iter = iterate(1, 10, mul_by_2);
	foreach(iter, print_by_space);
	
	puts("\n---------------------");
	save(list, "iter.txt");
	serialize(list, "siter.txt");
	struct list* empty1 = list_create_empty();
	struct list* empty2 = list_create_empty();
	load(&empty1, "iter.txt");
	deserialize(&empty2, "siter.txt");
	foreach(empty1, print_by_space);
	foreach(empty2, print_by_space);

	list_free(list_reverse);
	list_free(list);
	list_free(temp1);
	list_free(temp2);
	list_free(empty);
	list_free(empty1);
	list_free(empty2);
	list_free(iter);
	printf("\nhey");
}
