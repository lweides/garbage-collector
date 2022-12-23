#include "TypeDescriptor.h"

uint32_t next_pointer_offset(TypeDescriptor *descriptor, uint32_t *count) {
  uint32_t next_offset = (uint32_t) *((uint32_t*) descriptor + *count + 1);
  (*count)++;
  return next_offset;
}

TypeDescriptor* new_type_descriptor(uint32_t *offsets, uint32_t size, uint32_t pointer_count) {
  offsets[0] = size;
  offsets[pointer_count + 1] = SENTINEL;
  return (TypeDescriptor*) offsets;
}
