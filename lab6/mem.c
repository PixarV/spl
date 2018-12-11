#include "mem.h"
#include <string.h>
#include <stdbool.h>

#define MEM_T_SIZE sizeof(mem_t)
#define HEADER_PTR(ptr) (mem_t*) ((void*)ptr-MEM_T_SIZE)
#define BLOCK_PTR(ptr) ((void*)ptr+MEM_T_SIZE)

void memalloc_debug_struct_info(FILE *f, mem_t const *const address) {
    size_t i;
    fprintf(f,"start: %p\nsize: %lu\nis_free: %d\n",
            (void *) address,
            address->capacity,
            address->is_free);

    for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i)
        fprintf(f, "%hhX", ((char *) address)[MEM_T_SIZE + i]);
    putc('\n', f);
}

void memalloc_debug_heap(FILE *f, mem_t const *mem_ptr) {
    for (; mem_ptr; mem_ptr = mem_ptr->next)
        memalloc_debug_struct_info(f, mem_ptr);
}

static bool check_map_failed(void const* ptr, char const* msg, void const* addr) {
	if (!ptr || ptr == MAP_FAILED) {
		printf("%s on addr: %x\n", msg, addr);
		return false;
	}
	return true;
}

static void header_init(mem_t* header, mem_t* next, size_t size, bool is_free) {
	header->next = next;
	header->capacity = size;
	header->is_free = is_free;
}

static void* block_init(void* start, size_t size, int is_fixed) {
	void* block =  mmap(start, 
					 	size, 
					 	PROT_READ | PROT_WRITE, 
					 	MAP_ANONYMOUS | MAP_PRIVATE | is_fixed, 
					 	0, 0);
	if (!check_map_failed(block, "failed malloc block", start)) return NULL;

	mem_t* header = (mem_t*) block;
	header_init(header, NULL, size, false);

	return BLOCK_PTR(block);
}

static size_t get_required_size(size_t initial_size) {
	int one = initial_size % PAGE_SIZE == 0 ? 0 : 1;
	int page_count = initial_size / PAGE_SIZE + one;
	return page_count*PAGE_SIZE; 
}

static void* alloc_block(void* start, mem_t* next_header, size_t size) {
	mem_t* header = (mem_t*) start;
	header_init(header, next_header, size, false);
	return BLOCK_PTR(start);
}

static void set_header_next(void* block, mem_t* prev_header, mem_t* next_header) {
	mem_t* new_header = HEADER_PTR(block);
	if (prev_header) { prev_header->next = new_header; }
	new_header->next = next_header;
}

static void* alloc_more_heap(mem_t*  old_header, size_t query) {
	size_t required_size = get_required_size(query);
	void* block = block_init(NULL, required_size, 0);
	
	set_header_next(block, old_header, NULL);
	return block;
}

static void merge_free_blocks(mem_t* header) {
	mem_t* next_header = header->next;
	if (!next_header) return;
	
	mem_t* condition = (void*)header + MEM_T_SIZE + header->capacity;
	if (next_header->is_free && next_header == condition) {
		header->capacity += next_header->capacity;
		header->next = next_header->next;
	}
}

void* heap_init(size_t initial_size) {
	size_t required_size = get_required_size(initial_size);
	void* block = block_init(HEAP_START, required_size, MAP_FIXED);
	mem_t* header = HEADER_PTR(block);

	header->is_free = true;
	return block;
}

void* _malloc(size_t query) {
	mem_t* header = (mem_t*) HEAP_START;
	query = query < BLOCK_MIN_SIZE ? BLOCK_MIN_SIZE : query;

	while (header) {
		int condition = header->capacity - MEM_T_SIZE - BLOCK_MIN_SIZE;
		if (header->is_free && query <= condition) {
			return alloc_block(header, header->next, query);
		}

		if (!header->next) { break; }
		header = header->next;
	}

	void* start = (void*)header + MEM_T_SIZE + header->capacity; // after block end
	void* block = block_init(start, query, MAP_FIXED);

	if (!block) { 
		return alloc_more_heap(header, query);
	}
	
	set_header_next(block, header, NULL);
	return block;
}

void _free(void* block) {
	mem_t* header = HEADER_PTR(block);
	header->is_free = true;
	merge_free_blocks(header);
}
