#pragma once

typedef struct eatenBook {
  char* isbn;
  char* markBook;
  char* date;
} eatenBook;

void scantakenBooks(char*);
void addTakenBook(eatenBook*);
void addTakenStudent(eatenBook*);
eatenBook* eatHandedBook(FILE*, FILE*);
void giveBook(char*, char*);
void takeBook(char*, char*);
void printReaders(char*);
void printBooks(char*);
char* availableDate(char*);
int validDate(char*);
