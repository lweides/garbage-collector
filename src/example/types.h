#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_STUDENT_NAME 16
#define MAX_LECTURE_NAME 16

typedef struct StudentList {
  struct StudNode *first;
} StudentList;

static uint32_t STUDENT_LIST_DESCRIPTOR[] = {
  0,
  offsetof(StudentList, first),
  0
};

typedef struct StudNode {
  struct StudNode *next;
  struct Student *student;
} StudNode;

static uint32_t STUDENT_NODE_DESCRIPTOR[] = {
  0,
  offsetof(StudNode, next),
  offsetof(StudNode, student),
  0
};

typedef struct LectNode {
  struct LectNode *next;
  struct Lecture *lecture;
} LectNode;

static uint32_t LECTURE_NODE_DESCRIPTOR[] = {
  0,
  offsetof(LectNode, next),
  offsetof(LectNode, lecture),
  0
};

typedef struct Lecture {
  uint32_t id;
  char name[MAX_LECTURE_NAME];
  uint8_t semester;
} Lecture;

static uint32_t LECTURE_DESCRIPTOR[] = {
  0,
  0
};

typedef struct Student {
  uint32_t id;
  char name[MAX_STUDENT_NAME];
  LectNode *lectures;
} Student;

static uint32_t STUDENT_DESCRIPTOR[] = {
  0,
  offsetof(Student, lectures),
  0
};

/*
  Registers the types in the type registry
*/
void register_types();

/*
  Adds a student to the student list
*/
void add_student(StudentList *list, Student *student);

/*
  Removes a student from the student list
*/
void remove_student(StudentList *list, Student *student);

/*
  Adds a lecture to a student
*/
void add_lecture(Student *student, Lecture *lecture);

/*
  Removes a lecture from a student
*/
void remove_lecture(Student *student, Lecture *lecture);
