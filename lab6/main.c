#include "mem.h"

int main() {
    heap_init(PAGE_SIZE * 8);
    int *a, *b, *c, *d;
    memalloc_debug_heap(stdout, HEAP_START);
	a = _malloc(4076);
    *a = 42;
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	b = _malloc(1024);
	*b = 5;
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	c = _malloc(55);
	*c = 6;
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	_free(b);
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	d = _malloc(100);
	*d = 7;
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	_free(d);
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
	_free(a);
    memalloc_debug_heap(stdout, HEAP_START);
    printf("\n");
}
