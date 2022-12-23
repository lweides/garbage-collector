#pragma once

#include <stdint.h>
#include <stddef.h>

#include "TypeDescriptor.h"

#define MAX_TYPE_NAME_LENGTH 15

typedef struct TypeRegistryEntry {

  /*
    Points to the next entry of the TypeRegistry.
  */
  struct TypeRegistryEntry *next;

  /*
    The typeDescriptor of this entry.
    Will not be stored on the heap, thus is not part of the offset array
  */
  TypeDescriptor *descriptor;

  /*
    The name of the type.
    Can be a maximum of 10 characters long.
  */
  char name[MAX_TYPE_NAME_LENGTH + 1];
} TypeRegistryEntry;

TypeRegistryEntry *REGISTRY;

static uint32_t RAW_ENTRY_DESCRIPTOR[3] = {
  sizeof(TypeRegistryEntry),
  offsetof(TypeRegistryEntry, next),
  SENTINEL
};

static TypeDescriptor *ENTRY_TYPE_DESCRIPTOR = (TypeDescriptor*) RAW_ENTRY_DESCRIPTOR;

/*
  Initializes the type registry by allocating the initial TypeRegistryEntry.
*/
void init_type_registry();

/*
  Searches the registry for a type with the given name.
  The name has to be terminated with \0.
*/
TypeDescriptor* descriptor_by_name(char *name);

/*
  Searches through the registry and returns the name of the descriptor or NULL:
*/
char* name_of_descriptor(TypeDescriptor *descriptor);

/*
  Registers a new typeDescriptor and stores it in the registry.
*/
void register_descriptor(TypeDescriptor *descriptor, char *name);