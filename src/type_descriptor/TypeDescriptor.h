#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SENTINEL -1
#define EMPTY_BLOCK_DESCRIPTOR_SIZE_IN_BYTES 8

/*
  A typeDescriptor is at least 8 bytes in size:
    size                => 4 Bytes
    SENTINEL            => 4 Bytes
  More pointers are represented in the following way:
    size                => 4 Bytes
    pointer_offset_1    => 4 Bytes
    pointer_offset_2    => 4 Bytes
    ...                 => 4 Bytes * n
    SENTINEL            => 4 Bytes
*/
typedef struct TypeDescriptor {
  /*
    Size of the object described by this descriptor.
  */
  uint32_t size;
  /*
    The first pointer offset.
  */
  uint32_t pointer_offset;
} TypeDescriptor;

/*
  Returns the next pointer of the given typeDescriptor after count.
  The integer count may not be modified from outside the function,
  it is used internally to keep track of the current count.
*/
uint32_t next_pointer_offset(TypeDescriptor *descriptor, uint32_t *count);

/*
  Creates a new typeDescriptor from offsets and a size.
  Note that offsets has to be of length pointer_count + 2.
  The first offset has to be stored as offsets[1].

  As the memory of offsets is reused for the typeDescriptor, it may not be released afterwards.
*/
TypeDescriptor* new_type_descriptor(uint32_t *offsets, uint32_t size, uint32_t pointer_count);
