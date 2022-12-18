#include "utils.h"

void* add_offset_in_bytes(void* pointer, uint64_t offset_in_bytes) {
  return (void*) ((uintptr_t) pointer) + offset_in_bytes;
}

Block* block_from_pointer(void *pointer) {
  return (Block*) add_offset_in_bytes(pointer, -BLOCK_OVERHEAD_IN_BYTES);
}

void* pointer_from_block(Block *block) {
  return add_offset_in_bytes(block, BLOCK_OVERHEAD_IN_BYTES);
}
