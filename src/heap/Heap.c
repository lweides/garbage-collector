#include "Heap.h"
#include "MarkAndSweep.h"
#include "../type_descriptor/TypeRegistry.h"
#include "../utils.h"

#include <stdio.h>

void* alloc_by_name(char *name) {
  TypeDescriptor *descriptor = descriptor_by_name(name);
  return alloc(descriptor);
}

void* alloc(TypeDescriptor *descriptor) {
  uint32_t size = descriptor->size;
  void *pointer = alloc_size(size);
  Block *block = block_from_pointer(pointer);
  block->descriptor = descriptor;
  return pointer;
}

void* alloc_size(uint32_t size) {
  Block *prev = NULL;
  Block *current = HEAP->free_list;
  // search for fitting block
  while (current != NULL && block_too_small(current, size)) {
    prev = current;
    current = current->free.next;
  }

  if (current == NULL) {
    return NULL;
  }

  uint32_t new_block_size = current->descriptor->size - size - BLOCK_OVERHEAD_IN_BYTES;
  // split old block in two new blocks
  current->descriptor->size = new_block_size;
  Block *new_block = add_offset_in_bytes(current, new_block_size + BLOCK_OVERHEAD_IN_BYTES);

  clear(new_block, size);

  return pointer_from_block(new_block);
}

void gc(void **roots) {
  void *root;
  uint32_t i = 0;
  while ((root = roots[i++]) != NULL) {
    mark(root);
  }
  mark(REGISTRY);

  sweep(HEAP);
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

  init_type_registry();

  return true;
}

void free_heap() {
  // these are the only calls to free in the whole project
  free((Block*) HEAP->heap_start);
  free(HEAP); 
}

static bool block_too_small(Block *block, uint32_t size) {
  return ((int32_t) block->descriptor->size) - (int32_t) MIN_BLOCK_SIZE_IN_BYTES < (int32_t) size;
}

void dump() {
  printf("======== HEAP DUMP ========\n");

  uint32_t heap_size = HEAP->heap_size;
  uintptr_t heap_start = HEAP->heap_start;
  uintptr_t heap_end = heap_start + heap_size;
  printf("Heap starts at %#lx\n", heap_start);
  printf("Heap has size %d\n", heap_size);
  printf("Heap ends at %#lx\n\n", heap_end);

  printf("\n\n======== FREE LIST ========\n\n");
  Block *current = HEAP->free_list;
  uint32_t block_counter = 0;
  uint32_t size = 0;

  while (current != NULL) {
    TypeDescriptor *descriptor = current->descriptor;
    printf("======== FREE BLOCK ========\n");

    printf("Free block is located at %p\n", current);
    printf("Size of free block %d: %d bytes\n", block_counter, descriptor->size);

    printf("======== END FREE BLOCK ========\n");
    block_counter++;
    size += descriptor->size;
    current = current->free.next;
  }

  printf("\nCurrently %d bytes are free (and thus available)\n", size);

  printf("\n\n======== END FREE LIST ========\n\n\n");

  printf("======== ALL OBJECTS ========\n");
  block_counter = 0;
  current = (Block*) heap_start;
  size = 0;

  while ((uintptr_t) current < heap_end) {
    if (is_free(current)) {
      current = next_block(current);
      continue;
    }

    TypeDescriptor *descriptor = current->descriptor;

    void *pointer = pointer_from_block(current);
    uint8_t *first_byte = add_offset_in_bytes(pointer, 0);
    uint8_t *second_byte = add_offset_in_bytes(pointer, 1);
    uint8_t *third_byte = add_offset_in_bytes(pointer, 2);
    uint8_t *forth_byte = add_offset_in_bytes(pointer, 3);

    printf("\n======== OBJECT START ========\n");

    printf("Visiting object %d at %p in block at %p\n", block_counter, pointer, current);
    printf("Object %d has descriptor at: %p\n", block_counter, descriptor);
    printf("Object %d has size %d\n", block_counter, block_size(current));
    printf("Object %d is of type %s and is %d bytes large\n", block_counter, name_of_descriptor(descriptor), descriptor->size);
    printf("First 4 bytes:\n\t0x%02X\t0x%02X\t0x%02X\t0x%02X\n", *first_byte, *second_byte, *third_byte, *forth_byte);

    printf("======== OBJECT END ========\n");
    block_counter++;
    size += descriptor->size;
    current = next_block(current);
  }

  printf("\nObjects have a combined size of %d bytes and use %ld bytes\n\n", size, size + block_counter * BLOCK_OVERHEAD_IN_BYTES);
  printf("======== ALL OBJECTS END ========\n\n");
}