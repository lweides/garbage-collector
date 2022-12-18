#pragma once

#include "heap/Block.h"

#include <stdint.h>

/*
  Offsets the given pointer by offset_in_bytes.
*/
void* add_offset_in_bytes(void* pointer, uint64_t offset_in_bytes);

/*
  Returns the block, which holds the given pointer.
*/
Block* block_from_pointer(void *pointer);

/*
  Returns the pointer associated with this block.
*/
void* pointer_from_block(Block *block);