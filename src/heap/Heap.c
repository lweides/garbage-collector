#include "Heap.h"
#include "MarkAndSweep.h"
#include "../utils.h"

// void* alloc(TypeDescriptor *descriptor) {
//   Block *prev = HEAP;
//   Block *current = HEAP->free.next;
//   uint32_t size = descriptor->size;
//   // search for fitting block
//   while (block_too_small(current, size + BLOCK_OVERHEAD_IN_BYTES) && current != HEAP) {
//     prev = current;
//     current = current->free.next;
//   }

//   if (block_too_small(current, size + BLOCK_OVERHEAD_IN_BYTES)) {
//     return NULL;
//   }

//   uint32_t new_block_size = current->descriptor->size - size - BLOCK_OVERHEAD_IN_BYTES;
//   if (new_block_size <= MIN_BLOCK_SIZE_IN_BYTES) {
//     // resulting block would be too small, we use the whole block and accept the internal memory fragmentation
//     prev->free.next = current->free.next;
//     current->descriptor = descriptor;
//     // TODO set used_bit
//     // TODO zero bytes
//     return pointer_from_block(current);
//   }

//   // split old block in two new blocks
//   current->descriptor->size = new_block_size;
//   Block *new_block = add_offset_in_bytes(current, new_block_size);
//   new_block->descriptor = descriptor;
//   // TODO set used_bit
//   // TODO zero bytes
//   return pointer_from_block(new_block);
// }

void* alloc(TypeDescriptor *descriptor) {
  Block *prev = NULL;
  Block *current = HEAP->free_list;
  uint32_t size = descriptor->size;
  // search for fitting block
  while (current != NULL && block_too_small(current, size)) {
    prev = current;
    current = current->free.next;
  }

  if (current == NULL) {
    return NULL;
  }

  uint32_t new_block_size = current->descriptor->size - size - BLOCK_OVERHEAD_IN_BYTES;
  if (new_block_size <= MIN_BLOCK_SIZE_IN_BYTES) {
    // resulting block would be too small, we use the whole block and accept the internal memory fragmentation

    if (prev == NULL) {
      // we found a match at the first entry of the free list and need to replace it
      HEAP->free_list = current->free.next;
    } else {
      prev->free.next = current->free.next;
    }

    current->descriptor = descriptor;
    clear(current);
    
    return pointer_from_block(current);
  }

  // split old block in two new blocks
  current->descriptor->size = new_block_size;
  Block *new_block = add_offset_in_bytes(current, new_block_size + BLOCK_OVERHEAD_IN_BYTES);

  new_block->descriptor = descriptor;
  clear(new_block);

  return pointer_from_block(new_block);
}

void gc(void **roots) {
  void *root;
  uint32_t i = 0;
  while ((root = roots[i++]) != NULL) {
    mark(root);
  }
  sweep(HEAP);
}

void dealloc(void *pointer) {
  Block *block = block_from_pointer(pointer);
   Block *right = next_block(block);
  // TODO get left neighbour
  Block *current = HEAP->free_list;
  Block *prev = NULL;
  bool right_found_tombstone = false;
  while (current != NULL && current->free.next != block) {
    if (current == right) {
      right_found_tombstone = true;
    }
    prev = current;
    current = current->free.next;
  }

  if (current == NULL) {
    // left neighbour is not in free list, we add the block at the end
    // prev cannot be null, as that would mean that HEAP->free_list was null // TODO am I sure?
    current->free.next = prev->free.next;
    prev->free.next = current;
  }
  Block *left = NULL;
 
}

bool init_heap(uint32_t size) {
  // these are the only calls to calloc in the whole project
  Block *initial_block = (Block*) calloc(1, size);
  if (initial_block == NULL) {
    return false;
  }

  HEAP = calloc(1, sizeof(Heap));
  if (HEAP == NULL) {
    free(initial_block);
    return false;
  }

  HEAP->heap_start = (uintptr_t) initial_block;
  HEAP->heap_size = size;
  HEAP->free_list = initial_block;

  initial_block->free.next = NULL;
  initial_block->descriptor = add_offset_in_bytes(initial_block, sizeof(TypeDescriptor*));
  initial_block->descriptor->size = size - BLOCK_OVERHEAD_IN_BYTES;

  return true;
}

void free_heap() {
  // these are the only calls to free in the whole project
  free((Block*) HEAP->heap_start);
  free(HEAP); 
}

bool block_too_small(Block *block, uint32_t size) {
  return block->descriptor->size < size + BLOCK_OVERHEAD_IN_BYTES;
}

Heap* get_heap() {
  return HEAP;
}