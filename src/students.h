#pragma once

typedef struct studentBooks {
  char* isbn;
  char* date;
  struct studentBooks* next;
} studentBooks;

typedef struct student {
    char* markBook;
    char* firstName;
    char* secondName;
    char* thirdName;
    char* department;
    char* speciality;
    studentBooks* next;
    unsigned char height;
    struct student* left;
    struct student* right;
} student;

student* addStudent(student*, char*, char*, char*, char*, char*, char*);
void printStudents(student*);
void freeStudents(student*);
student* eatstudent(FILE*, FILE*);
student* removeStudent(student*, char*);
student* scanStudents(char*);
student* findStudent(student*, char* markBook) ;
void saveStudents(char*);
void saveStudent(student*, FILE*);
void studInfo(char*);
