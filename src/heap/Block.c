#include "Block.h"
#include "../utils.h"

bool is_marked(Block *block) {
  uintptr_t pointer = (uintptr_t) get_type_descriptor(block);
  return (pointer >> 1) & MARKED_BIT_MASK;
}

bool is_free(Block *block) {
  uintptr_t pointer = (uintptr_t) get_type_descriptor(block);
  return pointer == (uintptr_t) add_offset_in_bytes(block, BLOCK_OVERHEAD_IN_BYTES);
}

void set_marked(Block *block) {
  uintptr_t pointer = (uintptr_t) get_type_descriptor(block);
  block->descriptor = (TypeDescriptor*) (pointer | MARKED_BIT_MASK);
}

void clear_marked(Block *block) {
  uintptr_t pointer = (uintptr_t) get_type_descriptor(block);
  block->descriptor = (TypeDescriptor*) (pointer & ~MARKED_BIT_MASK);
}

void clear(Block *block, uint32_t size) {
  memset(pointer_from_block(block), 0, size);
}

uint32_t block_size(Block *block) {
  return get_type_descriptor(block)->size + BLOCK_OVERHEAD_IN_BYTES;
}

TypeDescriptor* get_type_descriptor(Block *block) {
  uintptr_t pointer = (uintptr_t) block->descriptor;
  return (TypeDescriptor*) (pointer & DESCRIPTOR_POINTER_MASK);
}

Block* next_block(Block *block) {
  return add_offset_in_bytes(block, block_size(block));
}
