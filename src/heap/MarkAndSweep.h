#pragma once

#include "Heap.h"

/*
  Traverses the given root, marking all traversed blocks.
*/
void mark(void *root);

/*
  Sweeps the heap, returning all used and not marked blocks to the free list.
*/
void sweep(Heap *heap);

void traverse_heap_debug(Heap *heap);