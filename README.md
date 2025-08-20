# Custom Memory Allocator (my_malloc)
**Overview**

August to December 2024, Systems Programming (CS 350)

This project implements a custom dynamic memory allocator in C, providing replacements for standard library functions like malloc() and free(). It demonstrates how memory can be managed manually inside the heap, including chunk splitting, coalescing adjacent free blocks, and managing a free list for efficiency.
By building my_malloc, I gained deeper insight into heap management, pointer arithmetic, memory alignment, and low-level systems programming concepts.

Features:
- Custom Allocation (my_malloc)
    - Allocates memory chunks from a managed heap.
    S- plits larger free blocks into smaller ones as needed.
- Custom Deallocation (my_free)
  - Returns memory chunks to the free list.
  - Coalesces adjacent free blocks to reduce fragmentation.
- Free List Management
  - Maintains a linked list of free memory chunks.
  - Tracks size and next free block (flink).
- Header Metadata
  - Each allocated block contains a header with size information.
  - Supports safe traversal and merging of blocks.
- Error Handling
  - Prevents invalid frees and overlapping allocations.
- Handles allocation failures gracefully.

**Included Files**
**my_malloc.c**

This file contains the full implementation of the custom memory allocator. It defines my_malloc() and my_free(), along with supporting functions for managing the free list, splitting and coalescing memory chunks, and validating block headers with a magic number. It represents the core logic of the project, handling all memory allocation and deallocation.

# my_malloc-driver.c

This driver file provides a testing harness for the allocator. It allocates and frees memory using my_malloc() and my_free(), checks alignment and header values, validates error handling, and prints out the state of the free list for debugging. It demonstrates how the allocator behaves under different scenarios and ensures correctness.
