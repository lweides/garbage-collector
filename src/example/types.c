#include "types.h"

#include "../type_descriptor/TypeDescriptor.h"
#include "../type_descriptor/TypeRegistry.h"
#include "../heap/Heap.h"


void register_types() {
  TypeDescriptor *student_list = new_type_descriptor(
    STUDENT_LIST_DESCRIPTOR,
    sizeof(StudentList),
    1
  );

  TypeDescriptor *student_node = new_type_descriptor(
    STUDENT_NODE_DESCRIPTOR,
    sizeof(StudNode),
    2
  );

  TypeDescriptor *lecture_node = new_type_descriptor(
    LECTURE_NODE_DESCRIPTOR,
    sizeof(LectNode),
    2
  );

  TypeDescriptor *lecture = new_type_descriptor(
    LECTURE_DESCRIPTOR,
    sizeof(Lecture),
    0
  );

  TypeDescriptor *student = new_type_descriptor(
    STUDENT_DESCRIPTOR,
    sizeof(Student),
    1
  );

  register_descriptor(student_list, "studentList");
  register_descriptor(student_node, "studentNode");
  register_descriptor(lecture_node, "lectureNode");
  register_descriptor(lecture, "lecture");
  register_descriptor(student, "student");
}

void add_student(StudentList *list, Student *student) {
  StudNode *node = alloc_by_name("studentNode");
  node->student = student;
  node->next = list->first;
  list->first = node;
}

void remove_student(StudentList *list, Student *student) {
  StudNode *current = list->first;
  StudNode *prev = NULL;

  while (current != NULL && current->student != student) {
    prev = current;
    current = current->next;
  }

  if (current == NULL) {
    // student not found
    return;
  }

  if (prev == NULL) {
    prev = list->first;
  }

  prev->next = current->next;
}

void add_lecture(Student *student, Lecture *lecture) {
  LectNode *node = alloc_by_name("lectureNode");
  node->lecture = lecture;
  node->next = student->lectures;
  student->lectures = node;
}

void remove_lecture(Student *student, Lecture *lecture) {
  LectNode *current = student->lectures;
  LectNode *prev = NULL;

  while (current != NULL && current->lecture != lecture) {
    prev = current;
    current = current->next;
  }

  if (current == NULL) {
    // lecture not found
    return;
  }

  if (prev == NULL) {
    prev = student->lectures;
  }

  prev->next = current->next;
}