#include "MarkAndSweep.h"
#include "../type_descriptor/TypeRegistry.h"
#include "../utils.h"

void mark(void *root) {
  Block* block = block_from_pointer(root);
  if (is_marked(block)) {
    return;
  }
  TypeDescriptor *descriptor = block->descriptor;  

  set_marked(block);

  uintptr_t heap_start = HEAP->heap_start;
  uintptr_t heap_end = heap_start + HEAP->heap_size;

  uint32_t counter = 0;
  uint32_t pointer_offset;
  while ((pointer_offset = next_pointer_offset(descriptor, &counter)) != SENTINEL) {
    uintptr_t *address_next_pointer = add_offset_in_bytes(root, pointer_offset);
    uintptr_t next_pointer = *address_next_pointer;
    if (heap_start <= next_pointer && next_pointer < heap_end) {
      mark((void*) next_pointer);
    }
  }
}

void sweep(Heap *heap) {

  heap->free_list = NULL;
  Block *current = (Block*) heap->heap_start;
  Block *prev = NULL;

  uint32_t heap_size = heap->heap_size;
  uint64_t heap_end = heap->heap_start + heap_size;
  
  while ((uintptr_t) current < heap_end) {
    if (is_marked(current)) {
      clear_marked(current);
      prev = current;
      current = next_block(current);
      continue;
    }

    uint32_t block_size = current->descriptor->size;
    current->descriptor = add_offset_in_bytes(current, sizeof(TypeDescriptor*));
    current->descriptor->size = block_size;

    if (prev != NULL && is_free(prev)) {
      prev->descriptor->size += current->descriptor->size + BLOCK_OVERHEAD_IN_BYTES;
      current = next_block(current);
      continue;
    }

    current->free.next = HEAP->free_list;
    HEAP->free_list = current;
    
    prev = current;
    current = next_block(current);
  }
}