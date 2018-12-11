#ifndef _MEM_H_
#define _MEM_H_

#define _USE_MISC

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/mman.h>
#include <unistd.h>

#define HEAP_START ((void*)0x04040000)
#define PAGE_SIZE getpagesize()
#define BLOCK_MIN_SIZE 256

#pragma pack(push, 1)
typedef struct mem_t {
	    struct mem_t* next;
	    size_t capacity;
	    int is_free;
} mem_t;
#pragma pack(pop)

void* _malloc(size_t query);
void _free(void* mem_t);
void* heap_init(size_t initial_size);

#define DEBUG_FIRST_BYTES 4

void memalloc_debug_struct_info(FILE* f, mem_t const *const address);
void memalloc_debug_heap(FILE* f, mem_t const* ptr);

#endif
