#include "MarkAndSweep.h"
#include "../utils.h"

#include <stdio.h>

void mark(void *root) {
  Block* block = block_from_pointer(root);
  if (is_marked(block)) {
    return;
  }
  TypeDescriptor *descriptor = block->descriptor;
  printf("Current pointer is %p\n", root);
  

  set_marked(block);

  // TODO mark current block

  uint32_t counter = 0;
  uint32_t pointer_offset;
  while ((pointer_offset = next_pointer_offset(descriptor, &counter)) != SENTINEL) {
    uintptr_t *address_next_pointer = add_offset_in_bytes(root, pointer_offset);
    printf("next_pointer: %p at %p\n", *address_next_pointer, address_next_pointer);
    if (*address_next_pointer == NULL) {
      continue;
    }
    mark(*address_next_pointer); // TODO replace with DSW algorithm, maybe something with levels?
    // TODO mark next_pointer and recurse
  }
}

void sweep(Heap *heap) {
  Block *current = (Block*) heap->heap_start;
  uint32_t heap_size = heap->heap_size;
  uint64_t heap_end = heap->heap_start + heap_size;
  
  while ((uintptr_t) current < heap_end) {
   
    if (!is_marked(current)) {
      dealloc(pointer_from_block(current));
    }

    current = next_block(current);
  }
}

void traverse_heap_debug(Heap *heap) {

  printf("======== HEAP START ========\n");

  uint32_t heap_size = heap->heap_size;
  uintptr_t heap_start = heap->heap_start;
  uintptr_t heap_end = heap_start + heap_size;
  printf("Heap starts at %#lx\n", heap_start);
  printf("Heap has size %d\n", heap_size);
  printf("Heap ends at %#lx\n", heap_end);

  uint32_t block_counter = 0;
  Block *current = heap->free_list;

  while ((uintptr_t) current < heap_end) {
    printf("\n======== BLOCK START ========\n");
    printf("Visiting block %d at %p\n", block_counter, current);
    printf("Distance to heap_end: %#lx\n", heap_end - (uintptr_t) current);
    printf("Block %d has descriptor at: %p\n", block_counter, current->descriptor);
    printf("Block %d has size %d\n", block_counter, block_size(current));
    printf("Block %d contains a type of size %d\n", block_counter, current->descriptor->size);
    

    printf("======== BLOCK END ========\n");
    block_counter++;
    current = next_block(current);
  }

  printf("\n======== HEAP END ========\n\n\n");
}