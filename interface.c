#include "interface.h"
#include "my_memory.h"

void my_setup(enum malloc_type type, int mem_size, void *start_of_memory){
    new_mem(type,mem_size,start_of_memory);
}

void *my_malloc(int size){
    void* start_ptr;
    
    //If Buddy Allocator
    if(get_type() == MALLOC_BUDDY){
        start_ptr = buddy_allocate(size);
        return start_ptr;
    }

    //If Slab Allocator
    else if(get_type() == MALLOC_SLAB){
        start_ptr = slab_allocate(size);
        return start_ptr;
    }

    //If not a vaild allocator type
    else
        return NULL;
}

void my_free(void *ptr){
    //If Buddy Allocator
    if(get_type() == MALLOC_BUDDY)
        buddy_free(ptr);

    //If Slab Allocator
    else if(get_type() == MALLOC_SLAB)
        slab_free(ptr);

    return;
}