#include "my_memory.h"

MEMORY my_memory;

///////////////////////////////////////////////////////////////////////////////
// Function     : new_mem
// Description  : Initialize new memory sctructure
//
// Inputs       : type: MALLOC_BUDDY or MALLOC_SLAB 
//                mem_size: size of memory
//                *start_of_memory: ptr to start of memory
//
// Outputs      : None

void new_mem(enum malloc_type type, int mem_size, void *start_of_memory){
    int i;

    //Set my_memory values
    my_memory.type = type;
    my_memory.size = mem_size;
    my_memory.mem_ptr = start_of_memory;
    my_memory.start_ptr = start_of_memory;

    //Create array for free memory
    for(i = 0;i <= POWERS_OF_TWO; i++){
        LIST* temp_list = (LIST*) malloc(sizeof(LIST));
        temp_list->head = NULL;
        temp_list->tail = NULL;
        temp_list->size = 0;
        my_memory.free_mem[i] = temp_list;
    }

    //Set free memory
    MEMORY_CHUNK* temp_chunk = (MEMORY_CHUNK*) malloc(sizeof(MEMORY_CHUNK));
    temp_chunk->size = MEMORY_SIZE;
    temp_chunk->start = 0;
    temp_chunk->end = MEMORY_SIZE;
    push(my_memory.free_mem[POWERS_OF_TWO],temp_chunk);

    //Create slab table if type slab
    if(type == MALLOC_SLAB){
        my_memory.slab_table = (LIST*) malloc(sizeof(LIST));
        my_memory.slab_table->head = NULL;
        my_memory.slab_table->tail = NULL;
        my_memory.slab_table->size = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function     : malloc_type
// Description  : Returns what type of malloc for memeory
//
// Inputs       : None
//
// Outputs      : malloc_type: MALLOC_BUDDY or MALLOC_SLAB

enum malloc_type get_type(){
    return my_memory.type;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : push
// Description  : Adds a memory chunk to end of a list
//
// Inputs       : list: ptr to a list
//                mem_chunk: ptr to memory chunk to add to list
//
// Outputs      : None

void push(LIST* list,MEMORY_CHUNK* mem_chunk){
    if(list->head == NULL){
        mem_chunk->next = NULL;
        list->head = mem_chunk;
        list->tail = mem_chunk;
    }
    else{
        mem_chunk->next = NULL;
        list->tail->next = mem_chunk;
        list->tail = mem_chunk;
    }
    list->size++;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : push_sort
// Description  : Adds a memory chunk to a list sorted by start position
//
// Inputs       : *list: ptr to a list
//                *mem_chunk: ptr to memory chunk to add to list
//
// Outputs      : None

void push_sort(LIST* list,MEMORY_CHUNK* mem_chunk){
    MEMORY_CHUNK* temp_chunk;
    MEMORY_CHUNK* prev_chunk;

    //If empty
    if(list->head == NULL){
        mem_chunk->next = NULL;
        list->head = mem_chunk;
        list->tail = mem_chunk;
    }
    //If sorts to beininng
    else if(mem_chunk->start < list->head->start){
        temp_chunk = list->head;
        mem_chunk->next = temp_chunk;
        list->head = mem_chunk;
    }
    //If size is one
    else if(list->size == 1){
        temp_chunk = list->head;
        //If last in list
        if(temp_chunk->start < mem_chunk->start){
            temp_chunk->next = mem_chunk;
            list->tail = mem_chunk;
            mem_chunk->next = NULL;
        }
    }
    //else
    else{
        temp_chunk = list->head->next;
        prev_chunk = list->head;
        //find placment
        while(temp_chunk != NULL && temp_chunk->start < mem_chunk->start){
            prev_chunk = temp_chunk;
            temp_chunk = temp_chunk->next;
        }

        prev_chunk->next = mem_chunk;
        mem_chunk->next = temp_chunk;
    }
    list->size++;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : pop
// Description  : Removes a memory chunk from the front of a list
//
// Inputs       : *list: ptr to a list
//
// Outputs      : MEMORY_CHUNK*: ptr to the memory chunk removed from the list

MEMORY_CHUNK* pop(LIST* list){
    MEMORY_CHUNK* temp_ptr;

    if(list->head == NULL){
        return NULL;
    }
    else{
        temp_ptr = list->head;
        list->head = temp_ptr->next;
        list->size--;
        return temp_ptr;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function     : pop_spec
// Description  : Removes a specific memory chunk from a list
//
// Inputs       : *list: ptr to a list
//                *mem_chunk: ptr to memory chunk to remove from list
//
// Outputs      : MEMORY_CHUNK*: ptr to the memory chunk removed from the list

MEMORY_CHUNK* pop_spec(LIST* list, MEMORY_CHUNK* mem_chunk){
    MEMORY_CHUNK* prev_chunk;

    if(mem_chunk == list->head){
        list->head = mem_chunk->next;
    }
    else{
        prev_chunk = list->head;
        while(prev_chunk->next != mem_chunk){
            prev_chunk = prev_chunk->next;
            if(prev_chunk == NULL)
                return NULL;
        }
        prev_chunk->next = prev_chunk->next->next;
    }
    list->size--;
    return mem_chunk;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : buddy_allocate
// Description  : Allocates a memory chunk of a givin size using buddy allocation policy
//
// Inputs       : size: size of memory chunk to allocate
//
// Outputs      : void*: ptr to start of memory chunk (NULL if can't allocate) 

void* buddy_allocate(int size){
    int* header;
    int i;
    int power_of_two_size;

    //Adds header size to needed memory size
    size += HEADER_SIZE;

    //If under min size set to min size
    if(size < MIN_MEM_CHUNK_SIZE)
        size = MIN_MEM_CHUNK_SIZE;

    //Rounds size to nect power of two
    size = pow(2,(int) ceil(log(size)/log(2)));
    
    //Size as next power of two
    power_of_two_size = (int) ceil(log(size)/log(2));

    //If no free mem of needed size
    if(my_memory.free_mem[power_of_two_size]->size == 0){
        //Search for next biggest chunk
        i = power_of_two_size;
        while(my_memory.free_mem[i]->size == 0){
            i++;
            //No chunk found
            if(i > POWERS_OF_TWO)
                return NULL;
        }

        //Divide chunk until needed size
        while(my_memory.free_mem[power_of_two_size]->size == 0){
            divide_chunk(i);
            i--;
        }
    }

    //Create header
    header = malloc(HEADER_SIZE);
    *header = size;

    //Return ptr
    MEMORY_CHUNK* temp_chunk = pop(my_memory.free_mem[power_of_two_size]);

    my_memory.mem_ptr = (char*)my_memory.mem_ptr - ((char*)my_memory.mem_ptr - (char*)my_memory.start_ptr);
    memcpy((int*)(((char*)my_memory.mem_ptr) + temp_chunk->start),header, HEADER_SIZE);
    my_memory.mem_ptr = (char*)my_memory.mem_ptr + HEADER_SIZE + temp_chunk->start;
    free(temp_chunk);

    return my_memory.mem_ptr;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : slab_allocate
// Description  : Allocates a memory chunk of a givin size using slab allocation policy
//
// Inputs       : size: size of memory chunk to allocate
//
// Outputs      : void*: ptr to start of memory chunk (NULL if can't allocate) 

void* slab_allocate(int size){
    MEMORY_CHUNK* slab;
    int* header;

    size += 8;

    //Get slab for size
    slab = get_slab(size);

    //If no slab for size
    if(slab == NULL){
        return NULL;
    }

    //Create header
    header = malloc(HEADER_SIZE);
    *header = size;

    //Return ptr
    MEMORY_CHUNK* temp_chunk = pop(slab->free_mem);

    slab->num_used++;
    my_memory.mem_ptr = (char*)my_memory.mem_ptr - ((char*)my_memory.mem_ptr - (char*)my_memory.start_ptr);
    memcpy((int*)(((char*)my_memory.mem_ptr) + temp_chunk->start),header, HEADER_SIZE);
    my_memory.mem_ptr = (char*)my_memory.mem_ptr + HEADER_SIZE + temp_chunk->start;
    free(temp_chunk);

    return my_memory.mem_ptr;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : buddy_free
// Description  : Frees a givin memory chunk using buddy allocation policy
//
// Inputs       : void*: ptr to start of memory chunk
//
// Outputs      : None 

void buddy_free(void *ptr){
    int start;
    int size;
    int power_of_two_size;
    int buddy_start;
    MEMORY_CHUNK* buddy;

    //Get start and size of mem chunk
    start = (char*)ptr - (char*)my_memory.start_ptr - HEADER_SIZE;
    size = *(int*)((char*)ptr-HEADER_SIZE);

    //Create chunk to add to free mem
    MEMORY_CHUNK* temp_chunk = (MEMORY_CHUNK*) malloc(sizeof(MEMORY_CHUNK));
    temp_chunk->size = size;
    temp_chunk->start = start;
    temp_chunk->end = start+size;

    //Add to free mem list
    power_of_two_size = (int) ceil(log(size)/log(2));
    push_sort(my_memory.free_mem[power_of_two_size],temp_chunk);

    //Get buddy
    buddy = check_buddy(my_memory.free_mem[power_of_two_size],temp_chunk);

    //If buddy merge
    while(buddy != NULL){
        temp_chunk = merge_buddy(my_memory.free_mem[power_of_two_size],temp_chunk,buddy);
        power_of_two_size++;
        if(power_of_two_size >= POWERS_OF_TWO)
            return;
        buddy = check_buddy(my_memory.free_mem[power_of_two_size],temp_chunk);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : slab_free
// Description  : Frees a givin memory chunk using slab allocation policy
//
// Inputs       : void*: ptr to start of memory chunk
//
// Outputs      : None 

void slab_free(void *ptr){
    MEMORY_CHUNK* slab;
    int start;
    int size;
    
    //Get start and size of mem chunk
    start = (char*)ptr - (char*)my_memory.start_ptr - HEADER_SIZE;
    size = *(int*)((char*)ptr-HEADER_SIZE);

    //Create chunk to add to free mem
    MEMORY_CHUNK* temp_chunk = (MEMORY_CHUNK*) malloc(sizeof(MEMORY_CHUNK));
    temp_chunk->size = size;
    temp_chunk->start = start;
    temp_chunk->end = start+size;

    //Find slab with chunk
    slab = my_memory.slab_table->head;
    while(start < slab->start){
        slab = slab->next;
    }

    //Set as free
    push_sort(slab->free_mem,temp_chunk);
    slab->num_used--;

    //If slab empty free it
    if(slab->num_used == 0){
        buddy_free(slab->slab_ptr);
        pop_spec(my_memory.slab_table,slab);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : divide_chunk
// Description  : Divides a free memory chunk of a givin size into 2 memory chunks 
//
// Inputs       : size: size of free memory chunk to split
//
// Outputs      : None 

void divide_chunk(int size){
    //Chunk to divide
    MEMORY_CHUNK* divide_chunk = pop(my_memory.free_mem[size]);

    //First split
    MEMORY_CHUNK* temp_chunk_one = (MEMORY_CHUNK*) malloc(sizeof(MEMORY_CHUNK));
    temp_chunk_one->size = divide_chunk->size/2;
    temp_chunk_one->start = divide_chunk->start;
    temp_chunk_one->end = divide_chunk->start+((divide_chunk->end-divide_chunk->start)/2);

    //Second split
    MEMORY_CHUNK* temp_chunk_two = (MEMORY_CHUNK*) malloc(sizeof(MEMORY_CHUNK));
    temp_chunk_two->size = divide_chunk->size/2;
    temp_chunk_two->start = divide_chunk->start+((divide_chunk->end-divide_chunk->start)/2);
    temp_chunk_two->end = divide_chunk->end;

    //Add to next lower list
    push(my_memory.free_mem[size-1],temp_chunk_one);
    push(my_memory.free_mem[size-1],temp_chunk_two);

    free(divide_chunk);
}

///////////////////////////////////////////////////////////////////////////////
// Function     : check_buddy
// Description  : Checks if a free memory chunks buddy is also free
//
// Inputs       : *list: ptr to list to check for buddy
//                *mem_chunk: memory chunk to check if buddy is free
//
// Outputs      : MEMORY_CHUNK*: ptr to buddy memory chunk (NULL if doesn't exist) 

MEMORY_CHUNK* check_buddy(LIST* list,MEMORY_CHUNK* mem_chunk){
    int buddy_start;
    int power_of_two_size;
        
    power_of_two_size = (int) ceil(log(mem_chunk->size)/log(2));

    //Get the start of buddy chunk
    if((mem_chunk->start/mem_chunk->size)%2 != 0){
        buddy_start = mem_chunk->start - mem_chunk->size;
    }
    else{
        buddy_start = mem_chunk->start + mem_chunk->size;
    }

    //Check if buddy chunk free
    MEMORY_CHUNK* buddy = my_memory.free_mem[power_of_two_size]->head;
    while(buddy != NULL && buddy->start != buddy_start){
        buddy = buddy->next;
    }
    return buddy;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : merge_buddy
// Description  : Merges two buddy memory chunks together
//
// Inputs       : *list: ptr to list that contains buddies
//                *mem_chunk_one: memory chunk of first buddy
//                *mem_chunk_two: memory chunk of second buddy
//
// Outputs      : MEMORY_CHUNK*: ptr to new merged memory chunk

MEMORY_CHUNK* merge_buddy(LIST* list,MEMORY_CHUNK* mem_chunk_one, MEMORY_CHUNK* mem_chunk_two){
    int power_of_two_size;

    //Merged chunk
    MEMORY_CHUNK* merge_chunk = malloc(sizeof(MEMORY_CHUNK));
    merge_chunk->size = mem_chunk_one->size*2;
    if(mem_chunk_one->start<mem_chunk_two->start){
        merge_chunk->start = mem_chunk_one->start;
        merge_chunk->end = mem_chunk_two->end;
    }
    else{
        merge_chunk->start = mem_chunk_two->start;
        merge_chunk->end = mem_chunk_one->end;
    }

    power_of_two_size = (int) ceil(log(mem_chunk_one->size)/log(2));

    //Pop chunks to be merged
    free(pop_spec(my_memory.free_mem[power_of_two_size],mem_chunk_one));
    free(pop_spec(my_memory.free_mem[power_of_two_size],mem_chunk_two));

    //Push new merged chunk
    push_sort(my_memory.free_mem[power_of_two_size+1],merge_chunk);

    return merge_chunk;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : get_slab
// Description  : Returns slab that has room for a memory chunk of givin size
//
// Inputs       : size: size of memory_chunk needed
//
// Outputs      : MEMORY_CHUNK*: ptr to slab that has room for memory chunk of size (NULL if no availble room) 

MEMORY_CHUNK* get_slab(int size){
    MEMORY_CHUNK* slab;

    slab = my_memory.slab_table->head;

    //Find slab for size
    while((slab != NULL && slab->type != size) || (slab != NULL && slab->type == size && slab->num_used == slab->num_obj)){
        slab = slab->next;
    }

    //Slab doesn't exsist
    if(slab == NULL){
        slab = new_slab(size);
        //Could not allocate
        if(slab == NULL){
            return NULL;
        }
        push_sort(my_memory.slab_table,slab);
    }
    return slab;
}

///////////////////////////////////////////////////////////////////////////////
// Function     : new_slab
// Description  : Returns new slab that has room for a memory chunk of givin size
//
// Inputs       : size: size of memory_chunk needed
//
// Outputs      : MEMORY_CHUNK*: ptr to slab that has room for memory chunk of size (NULL if couldn't allocate) 

MEMORY_CHUNK* new_slab(int size){
    MEMORY_CHUNK* new_slab;
    int i;

    //If too big to allocate
    if(size * N_OBJS_PER_SLAB > MEMORY_SIZE){
        return NULL;
    }

    //Create new slab
    new_slab = malloc(sizeof(MEMORY_CHUNK));
    new_slab->type = size;
    new_slab->size = N_OBJS_PER_SLAB * size;
    new_slab->num_obj = N_OBJS_PER_SLAB;
    new_slab->num_used = 0;
    new_slab->slab_ptr = buddy_allocate(new_slab->size);
    if(new_slab->slab_ptr == NULL)
        return NULL;
    new_slab->start = (char*)new_slab->slab_ptr - (char*)my_memory.start_ptr;
    new_slab->end = new_slab->start + new_slab->size;

    new_slab->free_mem = malloc(sizeof(LIST));
    new_slab->free_mem->head = NULL;
    new_slab->free_mem->tail = NULL;
    new_slab->free_mem->size = 0;

    for(i = 0; i < new_slab->num_obj; i++){
        MEMORY_CHUNK* temp_chunk = malloc(sizeof(MEMORY_CHUNK));
        temp_chunk->size = size;
        temp_chunk->start = new_slab->start + (new_slab->type * i);
        temp_chunk->end = temp_chunk->start + temp_chunk->size;

        push(new_slab->free_mem,temp_chunk);
    }
    return new_slab;
}