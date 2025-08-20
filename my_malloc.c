//
//  my_malloc.c
//  Lab1: Malloc
//

#include "my_malloc.h"
#include <stddef.h>
#include <unistd.h>

#define MIN_CHUNK 8192
#define MAGIC_NUMBER 44556677

MyErrorNo my_errno=MYNOERROR;

//Global variable to point to first node in the free list 
FreeListNode free_list = NULL;

//Insert_free_list helper function to insert a chunk into the free list according to address
void insert_free_list(FreeListNode chunk){
    FreeListNode *curr = &free_list;

    //Traverse the list to find the right place
    while (*curr != NULL && *curr < chunk) {
        curr = &(*curr)->flink;
    }

    //Insert the chunk
    chunk->flink = *curr;
    *curr = chunk;

    coalesce_free_list();
}

//Function to ensure chunk size is a multiple of 8 
uint32_t multiple_8(uint32_t size){
    return (size + 7) / 8 * 8;
}

//Function to get minuimum chunk size 
uint32_t min_chunk(){
    uint32_t freelistnode_size = sizeof(struct freelistnode);

    if(freelistnode_size > 16){
        return freelistnode_size;
    }else{
        return 16;
    }
}

//Find_chunk helper function: returns address of appropriately sized chunk to use (either from heap or free list)
FreeListNode find_chunk(uint32_t size){
    //realign size
    size = multiple_8(size);
    
    FreeListNode *curr = &free_list;

    //Traverse free list
    while(*curr != NULL){
        if((*curr)->size >= size + CHUNKHEADERSIZE){
            FreeListNode found_chunk = *curr;

            //Remove the found chunk from the list 
            *curr = found_chunk->flink;

            return found_chunk;
        }
        curr = &(*curr)->flink;
    }

    //If no chunk is found, request memory from the heap 
    uint32_t request_size = size;

    if(size + CHUNKHEADERSIZE > 8192) {
        request_size = size + CHUNKHEADERSIZE;
    }else{
        request_size = 8192;
    }

    //Create new node 
    FreeListNode new_chunk = (FreeListNode)sbrk(request_size);
    if(new_chunk == (void *)-1){
        return NULL;
    }
    
    new_chunk->size = request_size;
    new_chunk->flink = NULL;

    return new_chunk;

}

//Split chunk helper method and returns address of chunk to use 
FreeListNode split_chunk(FreeListNode chunk, uint32_t size){
    size = multiple_8(size);
    
    //Calculate size of the chunk that it not needed
    uint32_t remaining_size = chunk->size - (size + CHUNKHEADERSIZE);
    
    //Check if leftover chunk is big enough to store a new chunk
    if(remaining_size >= min_chunk() + CHUNKHEADERSIZE){
        //Calculates starting address of remainder chunk
        FreeListNode remainder = (FreeListNode)((uint32_t *)chunk + (size + CHUNKHEADERSIZE) / sizeof(__uint32_t));
        remainder->size = remaining_size;

        insert_free_list(remainder);

        //Update chunk size to match the part that is being given to the user 
        chunk->size = size + CHUNKHEADERSIZE;
    }

    return chunk;
}

void *my_malloc(uint32_t size)
{
    //If size to be allocated is 0
    if(size==0){
        my_errno = MYENOMEM;
        return NULL;
    }

    //Find/request chunk 
    FreeListNode chunk = find_chunk(size);

    //If there is no memory available 
    if(chunk==NULL){
        my_errno = MYENOMEM;
        return NULL;
    }

    //Split chunk if needed (if chunk is bigger than the amount of memory that is needed)
    //If you split it, you need to make sure the leftover chunk is big enough to store freelistnode structure
    if(chunk->size > size + CHUNKHEADERSIZE + min_chunk()){
        chunk = split_chunk(chunk, size);
    }

    //Pointer for where memory starts 
    //uint32_t *memory = (uint32_t *)((uint32_t *)chunk + 2);

    //Pointer for header
    uint32_t *header = (uint32_t*)chunk;
    //Store chunk size 
    header[0] = chunk->size;  
    //Store magic number 
    header[1] = MAGIC_NUMBER; 

    void *memory = (void *)(header + 2);
    //Return pointer to chunk (skip the header with +2)
    return (void *)(memory);
}
      
void my_free(void *ptr)
{
    if(ptr == NULL){
        my_errno = MYBADFREEPTR;
        return; 
    }

    //Get header 
    FreeListNode chunk = (FreeListNode)((uint32_t *)ptr-2);

    //Confirm magic number 
    if (*((uint32_t *)chunk + 1) != MAGIC_NUMBER) {
        my_errno = MYBADFREEPTR;  
        return;
    }

    //Clear magic number 
    *((uint32_t *)chunk + 1) = 0;

    chunk->flink = NULL;
    insert_free_list(chunk);

}

FreeListNode free_list_begin()
{
    return free_list;
    
}

int adjacent(FreeListNode chunk1, FreeListNode chunk2){
    //Cast to 32 bit integeter (access chunk1 as an array of 32 bit integers)
    uint32_t *chunk1_begin = (uint32_t *)chunk1;
    uint32_t chunk1_size = chunk1->size/sizeof(uint32_t);

    uint32_t *chunk1_end = chunk1_begin + chunk1_size;

    uint32_t *chunk2_begin = (uint32_t *)chunk2;

    return chunk1_end == chunk2_begin;
}

FreeListNode merge_chunks(FreeListNode chunk1, FreeListNode chunk2){
    if(adjacent(chunk1, chunk2)){
        //Combine the two chunks
        chunk1->size += chunk2->size;
        //Skip chunk 2 in the free list
        chunk1->flink = chunk2->flink;
    }

    return chunk1;
}

void coalesce_free_list()
{
    FreeListNode curr = free_list;

    while(curr != NULL && curr->flink != NULL){
        FreeListNode next = curr->flink;

        if(adjacent(curr,next)){
            curr = merge_chunks(curr,next);
        }else{
            curr = next;
        }
    }
}
