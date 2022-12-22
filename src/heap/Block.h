#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../type_descriptor/TypeDescriptor.h"

#define BLOCK_OVERHEAD_IN_BYTES sizeof(TypeDescriptor*)
#define MIN_BLOCK_SIZE_IN_BYTES (sizeof(TypeDescriptor*) + EMPTY_BLOCK_DESCRIPTOR_SIZE_IN_BYTES + sizeof(FreeBlock*))

#define MARKED_BIT_MASK 0b1
#define DESCRIPTOR_POINTER_MASK ~MARKED_BIT_MASK

typedef struct FreeBlock {
  TypeDescriptor descriptor;
  struct Block *next;
} FreeBlock;

typedef struct UsedBlock {
  /* data of block, size is given by descriptor.size */
} UsedBlock;

/*
  A free block has the following structure:

    address_of_descriptor | marked_bit      => 8 Bytes
    size_of_free_block                      => 4 Bytes
    SENTINEL                                => 4 Bytes
    pointer_to_next_free_block              => 8 Bytes
  
  Note that address_of_descriptor points ALWAYS to descriptor.

  A used block has the following structre:

    address_of_descriptor | marked_bit      => 8 Bytes
    data_of_object                          => descriptor->size Bytes

  Note that the data_of_object's length is equal to descriptor->size.
*/
typedef struct Block {
  TypeDescriptor *descriptor;
  union {
    FreeBlock free;
    UsedBlock used;
  };
} Block;

/*
  Returns true if the block is marked, false otherwise.
*/
bool is_marked(Block *block);

/*
  Returns true if the block is free, false otherwise.
  Being free can be determined from the typeDescriptor,
  if it points inside the Block, the block is considered free.
*/
bool is_free(Block *block);

/*
  Sets the marked bit of the block.
*/
void set_marked(Block *block);

/*
  Clears the marked bit of the block.
*/
void clear_marked(Block *block);

/*
  Clears the bytes managed by block.
*/
void clear(Block *block);

/*
  Returns the typeDescriptor installed in the block.
  Necessary because the marked bit modifies the address of the typeDescriptor.
  Used after blocks have been marked.
*/
TypeDescriptor* get_type_descriptor(Block *block);

/*
  Returns the block size, which is equal to the size of the type + BLOCK_OVERHEAD_IN_BYTES.
*/
uint32_t block_size(Block *block);

/*
  Returns a pointer to the next block, regardless of whether it is free or used.
*/
Block* next_block(Block *block);
