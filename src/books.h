#pragma once

typedef struct bookHolder {
  char* markBook;
  char* date;
  struct bookHolder* next;
} bookHolder;

typedef struct book {
    char* isbn;
    char* author;
    char* title;
    int totalBooks;
    int availableBooks;
    struct bookHolder* next;
    unsigned char height;
    struct book* left;
    struct book* right;
} book;

void printBook(book*);
void freeBook(book*);
book* eatbook(FILE*, FILE*);
book* removeBook(book*, char*);
book* scanBooks(char*);
book* addBook(book*, char*, char*, char*, int, int);
book* findBook(book*, char*);
void saveBooks(char*, char*);
void saveBook(book*, FILE*, FILE*);
void bookInfo(char*);
