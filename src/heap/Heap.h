#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "Block.h"


#define BLOCK_TO_POINTER_OFFSET BLOCK_OVERHEAD_IN_BYTES

/*
  Contains some meta data of the heap structure.
*/
typedef struct Heap {
  /*
    Address at which the heap starts in memory.
  */
  uintptr_t heap_start;

  /*
    Size of the heap in bytes.
  */
  uint32_t heap_size;

  /*
    First block of the free list.
  */
  Block *free_list;
} Heap;

/*
  The heap, containing additional information about the heap like size, etc.
  Has to be initialzed with init_heap(size) and freed using free_heap().
*/
static Heap *HEAP;

/*
  Allocates a new memory block with the given size and returns a pointer to it.
  Note that alloc will actually consume more memory than size, because of book-keeping.
*/
void* alloc(TypeDescriptor *descriptor);

/*
  Performs one iteration of mark and sweep.
  Roots cointains all currently live root pointers and is terminated by NULL.
*/
void gc(void **roots);

/*
  Frees the given pointer, returning it to the free list.
*/
void dealloc(void *pointer);

/*
  Initializes the heap with the given size.
  Returns false if the allocation fails, true otherwise.
*/
bool init_heap(uint32_t size);

/*
  Has to be called after the heap is no longer used.
  Frees the heap memory.
*/
void free_heap();

/*
  Returns true if the given block is too small to fit the given size.
*/
bool block_too_small(Block *block, uint32_t size);

Heap* get_heap();