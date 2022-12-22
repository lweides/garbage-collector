#include <stddef.h>

#include "TypeRegistry.h"
#include "Heap.h"
#include "../utils.h"

void init_type_registry() {
  register_descriptor(ENTRY_TYPE_DESCRIPTOR, "regEntryTyp");
}

TypeDescriptor* descriptor_by_name(char *name) {
  TypeRegistryEntry *current = REGISTRY;

  while (current != NULL) {
    if (!strcmp(name, current->name)) {
      return current->descriptor;
    }
    current = current->next;
  }
  return NULL;
}

char* name_of_descriptor(TypeDescriptor *descriptor) {
  TypeRegistryEntry *current = REGISTRY;

  while (current != NULL) {
    if (current->descriptor == descriptor) {
      return current->name;
    }
    current = current->next;
  }
  return NULL;
}

void register_descriptor(TypeDescriptor *descriptor, char *name) {
  if (strlen(name) > MAX_TYPE_NAME_LENGTH) {
    return;
  }

  TypeRegistryEntry *current = REGISTRY;
  TypeRegistryEntry *prev = NULL;

  while (current != NULL) {
    prev = current;
    current = current->next;
  }

  TypeRegistryEntry *new_entry = alloc(ENTRY_TYPE_DESCRIPTOR);
  new_entry->descriptor = descriptor;
  strcpy(new_entry->name, name);

  if (prev == NULL) {
     REGISTRY = new_entry;
  } else {
    prev->next = new_entry;
  }
}