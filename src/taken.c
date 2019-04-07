#include "global.h"
int handedBookNo=0;

static long int dateConvert(char*);

void scantakenBooks(char* fileName) {
  FILE *fileEat = fopen(fileName, "r");
  FILE *fileRead = fopen(fileName, "r");
  eatenBook *temp;
  while ((temp = eatHandedBook(fileEat, fileRead)) != NULL) {
    if ((findBook(bookRoot, temp->isbn)) && (findStudent(studentRoot, temp->markBook))) {
    addTakenBook(temp);
    addTakenStudent(temp);
    free(temp);}
  else if (!findBook(bookRoot, temp->isbn)) {
    printf("Found invalid ISBN %s, no such book in the database. Ignoring.\n", temp->isbn);
    printTime(); fprintf(logFile, " %s Found invalid ISBN %s, no such book in the database. Ignoring.\n", userLogin, temp->isbn);}
  else if (!findStudent(studentRoot, temp->markBook)) {
    printf("Found invalid student %s, no such markbok in the database. Ignoring.\n", temp->isbn);
    printTime(); fprintf(logFile, " %s Found invalid student %s, no such markbok in the database. Ignoring.\n", userLogin, temp->isbn);}
  }
  fclose(fileEat); fclose(fileRead);
}

void giveBook(char *isbn, char *markBook) {
  book* givenBook = findBook(bookRoot, isbn);
  if (givenBook == NULL) {
    printf("No book with ISBN %s was found.\n", isbn);
    printTime(); fprintf(logFile, " %s No book with ISBN %s was found.\n", userLogin, isbn);
    return; }
  student* givenStudent = findStudent(studentRoot, markBook);
  if (givenStudent == NULL) {
    printf("No student with ID %s was found.\n", markBook);
    printTime(); fprintf(logFile, " %s No student with ID %s was found.\n", userLogin, markBook);
    return; }
  printf("Found book:   ISBN %s,  title: %s\nFound student: ID %s,  surname: %s\n",
  isbn, givenBook->title, markBook, givenStudent->firstName);
  if (givenBook->availableBooks < 1) {
    printf("No books with ISBN %s are available.\n", isbn);
    if (givenBook->next != NULL) printf("The closest date one of the copies would be returned is %s\n", availableDate(isbn));
    return; }
  char *date = malloc(20);
  while (1) {
    printf("Enter date of return (DD.MM.YYYY):\n");
    fgets(date, 15, stdin); *(date+strlen(date)-1) = '\0';
    if (strcmp(date, "exit") == 0) return;
    if (!validDate(date)) {
      printf("Invalid format. Please try again or type \"exit\".\n");
      printTime(); fprintf(logFile, " %s Invalid format of date. Please try again or type \"exit\".\n", userLogin);}
    else break; }
  bookHolder *newReader = malloc(sizeof(bookHolder)), *tempReader = givenBook->next;
  studentBooks *newBook = malloc(sizeof(studentBooks)), *tempBook = givenStudent->next;
  newReader->markBook = markBook; newReader->date = date; newReader->next = NULL;
  newBook->isbn = isbn; newBook->date = date; newBook->next = NULL;
  if (tempReader) {
    while (tempReader->next != NULL) tempReader = tempReader->next;
    tempReader->next = newReader;}
  else givenBook->next = newReader;
  if (tempBook) {
    while (tempBook->next != NULL) tempBook = tempBook->next;
    tempBook->next = newBook; }
  else givenStudent->next = newBook;
  givenBook->availableBooks--;
  printf("Book %s was successfully given out to student %s, date of return: %s.\n",
    isbn, markBook, date);
  printTime(); fprintf(logFile, " %s Book %s was successfully given out to student %s, date of return: %s.\n",
     userLogin, isbn, markBook, date);
}

void takeBook(char* isbn, char* markBook) {
  int cmp=1;
  char *date;
  book* returningBook = findBook(bookRoot, isbn);
  if (returningBook == NULL) {
    printf("No book with ISBN %s was found.\n", isbn);
    printTime(); fprintf(logFile, " %s No book with ISBN %s was found.\n", userLogin, isbn);
    return;}
  student* returningStudent = findStudent(studentRoot, markBook);
  if (returningStudent == NULL) {
    printf("No student with ID %s was found.\n", markBook);
    printTime(); fprintf(logFile, " %s No student with ID %s was found.\n", userLogin, markBook);
    return;}
  bookHolder *prevStud = returningBook->next, *tempStud = returningBook->next;
  studentBooks *prevBook = returningStudent->next, *tempBook = returningStudent->next;
  if (tempBook == NULL) {
    printf("This book %s hasn't been taken by anybody.\n", isbn);
    printTime(); fprintf(logFile, " %s This book %s hasn't been taken by anybody.\n", userLogin, isbn);
    return;}
  if (tempStud == NULL) {
    printf("This student %s hasn't taken anything.\n", markBook);
    printTime(); fprintf(logFile, " %s This student %s hasn't taken anything.\n", userLogin, markBook);
    return;}

    while (tempStud->next) {
      if ((cmp = strcmp(tempStud->markBook, markBook)) == 0) {
        date = tempStud->date;
        break; }
      prevStud = tempStud;
      printf("Found student %s\n", tempStud->markBook);
      tempStud = tempStud->next;
    }
  if (cmp != 0) {printf("This book wasn't taken by this student!\n"); return;}

  while (1) {
    if ((cmp = strcmp(tempBook->isbn, isbn)) == 0) {
      date = tempBook->date;
      break; }
    prevBook = tempBook;
    printf("Found book %s\n", tempBook->isbn);
    if (tempBook->next == NULL) break;
    tempBook = tempBook->next;
  }
  if (cmp != 0) {printf("This student haven't taken this book!\n"); return;}

  char act[10];
  printf("Found student %s, who took book %s and has to return it till %s\nRight?\n",
  markBook, isbn, date);
  while (1) {
    printf("Type \"y\" or \"n\"\n");
    fgets(act, 8, stdin); *(act+strlen(act)-1) = '\0';
    if (!(strcmp(act, "n"))) return;
    if (!(strcmp(act, "y"))) break;}
  prevBook->next = tempBook->next;
  prevStud->next = tempStud->next;
  returningBook->availableBooks++;
  printf("Book %s was successfully taken back.\n", isbn);
  printTime(); fprintf(logFile, " %s Book %s was successfully taken back.\n", userLogin, isbn);
  if (returningBook->availableBooks > returningBook->totalBooks) {
    printf("SIC! According to the datebases, the library has now more available copies \n");
    printf("of the book with ISBN %s than total amount of such books.\n", isbn);
    printf("Total books: %d, available %d.\n", returningBook->totalBooks, returningBook->availableBooks);
    printf("Please, pay attention to this fact.\n"); }
}

void addTakenBook(eatenBook* temp) {
  book *book = findBook(bookRoot, temp->isbn);
  if (book == NULL) {
    printf("Found invalid ISBN %s, no such book in the database. Ignoring.\n", temp->isbn);
    return; }
  bookHolder* reader = (bookHolder*)malloc(sizeof(bookHolder));
  reader->markBook = temp->markBook;
  reader->date = temp->date;
  reader->next = NULL;
  if (book->next == NULL) {
    book->next = reader;}
  else {
    bookHolder *pnt = book->next;
    while (pnt->next != NULL) pnt = pnt->next;
    pnt->next = reader;}
  book->availableBooks--;
}

void printReaders(char* isbn) {
    book* book = findBook(bookRoot, isbn);
    if (book == NULL) {printf("No book with ISBN %s was found.\n", isbn); return;}
    if (book->next == NULL) {printf("Nobody has taken this book yet.\n"); return;}
    bookHolder* tempPointer = book->next;
    printf("This book (ISBN %s) was taken by the following students:\n", isbn);
    printf("ID of markbook  Date of return\n");
    while (tempPointer) {
      printf("%-17s%s\n", tempPointer->markBook, tempPointer->date);
      tempPointer = tempPointer->next; }
}

void addTakenStudent(eatenBook* temp) {
  student *student = findStudent(studentRoot, temp->markBook);
  if (student == NULL) {
    printf("Found invalid student %s, no such markbook in the database. Ignoring.\n", temp->isbn);
    return; }
  studentBooks* book = (studentBooks*)malloc(sizeof(studentBooks));
  book->isbn = temp->isbn;
  book->date = temp->date;
  book->next = NULL;
  if (student->next == NULL) {
    student->next = book; }
  else {
    studentBooks *pnt = student->next;
    while (pnt->next != NULL) pnt = pnt->next;
    pnt->next = book; }
}

void printBooks(char* markBook) {
    student* student = findStudent(studentRoot, markBook);
    if (!student) {
      printf("No student with markbook ID %s was found.\n", markBook);
      return;}
    if (student->next == NULL) {printf("This student hasn't taken anything yet.\n"); return;}
    studentBooks* tempPointer = student->next;
    printf("Student %s took the following books:\n", markBook);
    printf("ISBN         Date of return\n");
    while (tempPointer) {
      printf("%-s   %s\n", tempPointer->isbn, tempPointer->date);
      tempPointer = tempPointer->next; }
}

int validDate(char* date) {
  int days, months, years;
  char day[3], month[3], year[5];
  if ((*(date+2) != '.') || (*(date+5) != '.') || (*(date+10) != '\0')) return 0;
  day[0] = *(date); day[1] = *(date+1); day[2] = '\0';
  days = atoi(day);
  if (days > 31) return 0;
  month[0] = *(date+3); month[1] = *(date+4); month[2] = '\0';
  months = atoi(month);
  if (months > 12) return 0;
  year[0] = *(date+6); year[1] = *(date+7); year[2] = *(date+8);
  year[3] = *(date+9); year[4] = '\0';
  years = atoi(year);
  if (years > 2100) return 0;
  return 1;
}

char* availableDate(char* isbn) {
    book *book = findBook(bookRoot, isbn);
    bookHolder *reader = book->next;
    long int tempDate, min = dateConvert(reader->date);
    while (reader) {
      tempDate = dateConvert(reader->date);
      if (tempDate < min) min = tempDate;
      if (reader->next == NULL) break;
      reader = reader->next; }
    return reader->date;
}

static long int dateConvert(char *date) {
  int days, months, years;
  long int intDate;
  char day[3], month[3], year[5];

  day[0] = *(date); day[1] = *(date+1); day[2] = '\0';
  days = atoi(day);
  month[0] = *(date+3); month[1] = *(date+4); month[2] = '\0';
  months = atoi(month);
  year[0] = *(date+6); year[1] = *(date+7); year[2] = *(date+8);
  year[3] = *(date+9); year[4] = '\0';
  years = atoi(year);
  intDate = years*365 + months*30 + days;
  return intDate;
}

eatenBook* eatHandedBook(FILE* fileEat, FILE* fileRead) {
    int i=0, c=0, length=0;
    char *isbn = NULL, *markBook = NULL, *date = NULL;
    eatenBook *boookPntr = malloc(sizeof(eatenBook));

        while ((c = fgetc(fileRead)) != ';') {
          if (c == EOF) {free(boookPntr); return NULL;}
          length++; }
        isbn = malloc(length+1);
        for (i = 0; i <= length; i++) {
            *(isbn+i) = fgetc(fileEat);
            if (*(isbn+i) == ';') {
                *(isbn+i) = '\0';
                break; }
            }
        boookPntr->isbn = isbn;

        length=0;
        while ((c = fgetc(fileRead)) != ';') {
            length++; }
        markBook = malloc(length+1);
        for (i = 0; i <= length; i++) {
            *(markBook+i) = fgetc(fileEat);
            if (*(markBook+i) == ';') {
                *(markBook+i) = '\0';
                break; }
            }
        boookPntr->markBook = markBook;

        length=0;
        while ((c = fgetc(fileRead)) != '\n') {
            length++; }
        date = malloc(length+1);

        for (i = 0; i <= length; i++) {
            *(date+i) = fgetc(fileEat);
            if (*(date+i) == '\n') {
                *(date+i) = '\0';
                break; }
            }
        boookPntr->date = date;
    return boookPntr;
  }
