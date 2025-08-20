# Custom Memory Allocator (my_malloc)
# Overview

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
