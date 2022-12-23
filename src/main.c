#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "heap/Heap.h"
#include "example/types.h"
#include "heap/MarkAndSweep.h"

int main() {
  init_heap(32 * 1024);

  printf("Heap state after init_heap\n");
  dump();

  register_types();

  Lecture *system_software = alloc_by_name("lecture");
  system_software->id = 0;
  strcpy(system_software->name, "SystemSoftware");
  system_software->semester = 7;

  Lecture *compiler_construction = alloc_by_name("lecture");
  compiler_construction->id = 1;
  strcpy(compiler_construction->name, "CompConstr");
  compiler_construction->semester = 5;

  Lecture *software_development_1 = alloc_by_name("lecture");
  software_development_1->id = 2;
  strcpy(software_development_1->name, "softDevel1");
  software_development_1->semester = 1;

  Lecture *software_development_2 = alloc_by_name("lecture");
  software_development_2->id = 3;
  strcpy(software_development_2->name, "softDevel2");
  software_development_2->semester = 2;

  // define students
  Student *lukas = alloc_by_name("student");
  lukas->id = 0;
  strcpy(lukas->name, "Lukas");

  Student *elias = alloc_by_name("student");
  elias->id = 1;
  strcpy(elias->name, "Elias");

  Student *tobias = alloc_by_name("student");
  tobias->id = 2;
  strcpy(tobias->name, "Tobias");

  // add lectures to students
  add_lecture(lukas, software_development_1);
  add_lecture(lukas, compiler_construction);

  add_lecture(elias, software_development_2);
  add_lecture(elias, system_software);

  add_lecture(tobias, compiler_construction);
  add_lecture(tobias, system_software);

  // add students to list
  StudentList *list = alloc_by_name("studentList");
  add_student(list, lukas);
  add_student(list, elias);
  add_student(list, tobias);

  void *roots[] = { list, NULL };

  printf("Heap state after initialization of data structure\n");
  dump();

  // we remove elias from the list
  remove_student(list, elias);
  gc(roots);

  printf("\n\nHeap state after removal of elias\n");
  dump();

  // we remove the lecture software_development_1
  remove_lecture(lukas, software_development_1);
  gc(roots);

  printf("\n\nHeap state after removal of software_development_1\n");
  dump();

  void *roots_without_list[] = { NULL };
  gc(roots_without_list);

  printf("\n\nHeap state after removal of the whole list\n");
  dump();

  free_heap();
  return 0;
}
