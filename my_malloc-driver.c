//
//  my_malloc-driver.c
//  Lab1: Malloc
//
#include "my_malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAGIC_NUMBER 44556677

void print_header(void *ptr) {
    uint32_t *mem = (uint32_t *)ptr;
    uint32_t *header = (uint32_t *)ptr - 2;  // Move back by 2 to access the header
    printf("Header Information:\n");
    printf("\tPointer: %p\n", ptr);
    printf("\tChunk Size: %u\n", header[0]);  // The size of the chunk
    printf("\tMagic Number: %u\n", header[1]);  // The stored magic number
}

// Function to print the free list
void print_free_list(FreeListNode head) {
    printf("Free List:\n");
    FreeListNode current = head;
    while (current != NULL) {
        printf("\tChunk Size: %u, Address: %p, Next: %p\n", current->size, current, current->flink);
        current = current->flink;  // Move to the next node
    }
}



int main(int argc, const char * argv[])
{
    /*
   for(int i=1; i<10;i++){
    void *mem;
    printf("callling my_malloc(%d)\n", (i*1000));
    mem = my_malloc(i*1000);

    printf("Before zeroing bytes:\n");

    //print_header(mem);
    print_free_list(free_list_begin());
    memset(mem, 0, i*1000);
    printf("After zeroing bytes:\n");

    //my_free(mem);

    print_free_list(free_list_begin());
    printf("After freeing:\n");
    my_free(mem);
    print_free_list(free_list_begin());

   }
   

   void *ptr = my_malloc(32); // Allocate 32 bytes
    
    if (!ptr) {
        printf("Allocation failed!\n");
        return 1;
    }

    // Get the header from the allocated chunk
    uint32_t *header = (uint32_t *)ptr - 2;

    // Check if the first value in the header is the size
    printf("Header Size: %u\n", header[0]);

    // Check if the second value in the header is the magic number
    printf("Magic Number: %u\n", header[1]);

    // Validate if the header exists
    if (header[1] != MAGIC_NUMBER) {
        printf("Error: Chunk header missing or corrupted!\n");
    } else {
        printf("✅ Chunk header is correctly set!\n");
    }

    if ((uintptr_t)ptr % 8 != 0) {
        printf("Error: Allocated memory is not aligned to 8 bytes!\n");
    }

    my_free(ptr); // Free memory
    return 0;
    

    void *ptr = my_malloc(64);
    my_free(ptr);

    // Try reading the magic number after freeing
    uint32_t *header = (uint32_t *)ptr - 2;
    if (header[1] == MAGIC_NUMBER) {
        printf("Error: Magic number not cleared after free!\n");
    }   else {
        printf("✅ Magic number properly cleared after free!\n");
    }
   */ 

    void *ptr = my_malloc(32); // Allocate 32 bytes

    if (!ptr) {
        printf("Allocation failed!\n");
        return 1;
    }

    // Check the header values
    uint32_t *header = (uint32_t *)ptr - 2;
    
    printf("Header Size: %u\n", header[0]);
    printf("Magic Number: %u\n", header[1]);

    if (header[1] != MAGIC_NUMBER) {
        printf(" Error: Header is corrupted!\n");
    } else {
        printf("Header is correctly set!\n");
    }

    my_free(ptr);
    return 0;
   
    


    
}
