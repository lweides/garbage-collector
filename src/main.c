#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "heap/Heap.h"
#include "type_descriptor/TypeDescriptor.h"
#include "type_descriptor/TypeRegistry.h"
#include "heap/MarkAndSweep.h"

typedef struct List {
  uint32_t value;
  struct List *next;
  uint32_t dummy;
} List;

int main() {
  init_heap(32 * 1024);

  traverse_heap_debug(get_heap());

  printf("Setup list desc\n");
  uint32_t offsets[] = { 0, offsetof(List, next), 0 };
  uint32_t size = sizeof(List);
  uint32_t pointer_count = 1;
  TypeDescriptor *list_descriptor = new_type_descriptor(offsets, size, pointer_count);
  register_descriptor(list_descriptor, "listDesc");
  List *list = alloc_by_name("listDesc");

  List *current = list;

  for (int i = 0; i < 5; i++) {
    current->value = i;
    current->next = alloc_by_name("listDesc");

    current = current->next;
  }

  current->value = 5;

  traverse_heap_debug(get_heap());

  current = list;

  // while (current != NULL) {
  //   printf("%d\n", current->value);
  //   current = current->next;
  // }

  mark(list);

  free_heap();
  return 0;
}

