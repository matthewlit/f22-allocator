#ifndef MY_MEMORY_H
#define MY_MEMORY_H

#include "interface.h"

#define POWERS_OF_TWO 23

typedef struct LIST
{
    struct MEMORY_CHUNK* head;
    struct MEMORY_CHUNK* tail;
    int size;
} LIST;

typedef struct MEMORY_CHUNK
{
    int size;
    int start;
    int end;
    struct MEMORY_CHUNK* next;
    int type;
    int num_obj;
    int num_used;
    LIST* free_mem;
    void* slab_ptr;
} MEMORY_CHUNK;

typedef struct MEMORY
{
    enum malloc_type type;
    int size;
    void* mem_ptr;
    void* start_ptr;
    LIST* free_mem[POWERS_OF_TWO+1];
    LIST* slab_table; 
} MEMORY;

void new_mem(enum malloc_type type, int mem_size, void *start_of_memory);
enum malloc_type get_type();
void push(LIST* list, MEMORY_CHUNK* mem_chunk);
void push_sort(LIST* list, MEMORY_CHUNK* mem_chunk);
MEMORY_CHUNK* pop(LIST* list);
MEMORY_CHUNK* pop_spec(LIST* list, MEMORY_CHUNK* mem_chunk);
void* buddy_allocate(int size);
void* slab_allocate(int size);
void buddy_free(void *ptr);
void slab_free(void *ptr);
void divide_chunk(int size);
MEMORY_CHUNK* check_buddy(LIST* list,MEMORY_CHUNK* mem_chunk);
MEMORY_CHUNK* merge_buddy(LIST* list,MEMORY_CHUNK* mem_chunk_one, MEMORY_CHUNK* mem_chunk_two);
MEMORY_CHUNK* get_slab(int size);
MEMORY_CHUNK* new_slab(int size);

#endif
