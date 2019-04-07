#include "global.h"

void auth() {
    char login[50], password[50];
    user *userAcc;
    while (1) {
      printf("Please, enter your username (or type \"exit\" to exit):\n");
      fgets(login, 20, stdin); *(login+strlen(login)-1) = '\0';
      if (strcmp(login, "exit") == 0) return;
      userAcc = findUser(usersRoot, login);
      if (userAcc == NULL) printf("Invalid username\n");
      else  {
        printf("Password:\n");
        fgets(password, 20, stdin); *(password+strlen(password)-1) = '\0';
        if (strcmp(userAcc->password, password) == 0) break;
        else printf("Invalid password\n");}
    }
    userLogin = login;
    int stdAcc = userAcc->studentAccess;
    int bkAcc = userAcc->booksAccess;
    printTime(); fprintf(logFile, " %s Logged in.\n", userLogin);
    if ((stdAcc == 1) && (bkAcc == 1)) {userClass = 0; system("clear"); menu(); return;}
      else if ((stdAcc == 1) && (bkAcc == 0)) {userClass = 1; system("clear"); editStudent();}
      else if ((stdAcc == 0) && (bkAcc == 1)) {userClass = 2; system("clear"); editBook();}
      else if ((stdAcc == 0) && (bkAcc == 0)) printf("You don't have ANY rights! Sad, but true.\n");
    if (clearmode == 1) system("clear");
}

void menu() {
  if (clearmode == 1) system("clear");
  if (checkExit == 1) return;
  char action, trash;
  printf("Hello, admin! What we are doing tonight?\n");
  printf("1 - Enter Book Menu\n2 - Enter Students Menu\n0 - Exit\n");
  action = getchar();
  while ((trash = getchar()) != '\n');
  switch (action) {
    case '1':
    	printTime(); fprintf(logFile, " %s Entered books menu.\n", userLogin);
      if (clearmode == 1) system("clear");
    	editBook(); break;
    case '2':
    	printTime(); fprintf(logFile, " %s Entered students menu.\n", userLogin);
      if (clearmode == 1) system("clear");
    	editStudent(); break;
    case '0':
    	printTime(); fprintf(logFile, " %s Exited program.\n", userLogin);
    	checkExit = 1; printf("Exiting...\n");
      if (clearmode == 1) system("clear"); return;
  }
  menu();
}

void editBook(){
  if (checkExit == 1) return;
  if (userClass!=0) printf("Hello, %s!\n", userLogin);
  printf("What would you like to do?\n1 - Edit book\n2 - Add book\n3 - Print all books\n4 - See book's info\n5 - Take book from a student\n6 - Give student a book\n7 - Check out who has taken a book\n8 - Save\n9 - Exit\n");
  if (userClass == 0) printf("0 - Back\n");
  char action, trash;
  action = getchar();
  while ((trash = getchar()) != '\n');
  char *isbn = NULL, *markBook = NULL;
  switch (action) {
    case '1':
      printf("Enter ISBN of a book.\n");
      isbn = malloc(22);
      fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
      printTime(); fprintf(logFile, " %s Editing ISBN %s.\n", userLogin, isbn);
      book* searchRes = NULL;
      if ((searchRes = findBook(bookRoot, isbn)) == NULL) {error(3); break;}
      if (clearmode == 1) system("clear");
      printf("Here's book info:\n");
      bookInfo(isbn);
      editBookMenu(searchRes);
      free(isbn);
      if (clearmode == 1) system("clear");
      break;
  case '2':
    if (clearmode == 1) system("clear");
  	printTime(); fprintf(logFile, " %s Adding new book.\n", userLogin);
  	addBookCons(); break;
  case '3':
    if (clearmode == 1) system("clear");
  	printTime(); fprintf(logFile, " %s Printed books.\n", userLogin);
  	printBook(bookRoot); printf("\n"); break;
  case '4':
    if (clearmode == 1) system("clear");
    isbn = malloc(22);
    printf("Enter ISBN of a book.\n");
    fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
    if (findBook(bookRoot, isbn) == NULL) {error(3); break;}
    bookInfo(isbn);
    free(isbn);
    break;
  case '5':
    if (clearmode == 1) system("clear");
    printf("Enter ISBN of the book:\n");
    isbn = malloc(22); fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
    printf("Enter ID of the student's markBook:\n");
    markBook = malloc(12); fgets(markBook, 10, stdin); *(markBook+strlen(markBook)-1) = '\0';
    printTime(); fprintf(logFile, " %s Taking book %s from a student %s.\n", userLogin, isbn, markBook);
    if (clearmode == 1) system("clear");
    takeBook(isbn, markBook);
    free(isbn); free(markBook);
    break;
  case '6':
    if (clearmode == 1) system("clear");
    printf("Enter ISBN of the book:\n");
    isbn = malloc(22); fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
    printf("Enter ID of the student's markBook:\n");
    markBook = malloc(12); fgets(markBook, 10, stdin); *(markBook+strlen(markBook)-1) = '\0';
    printTime(); fprintf(logFile, " %s Giving book %s to a student %s.\n", userLogin, isbn, markBook);
    if (clearmode == 1) system("clear");
    giveBook(isbn, markBook);
    free(isbn); free(markBook);
    break;
  case '7':
    printf("Enter ISBN of the book:\n");
    isbn = malloc(22); fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
    printTime(); fprintf(logFile, " %s Checking readers of the book %s.\n", userLogin, isbn);
    if (clearmode == 1) system("clear");
    printReaders(isbn);
    free(isbn);
    break;
  case '8':
  	printTime(); fprintf(logFile, " %s Saved books.\n", userLogin);
  	saveBooks(booksFile, studentBooksFile); printf("Saved.\n"); break;
  case '9':
    if (clearmode == 1) system("clear");
    printf("Exiting...\n"); checkExit = 1;
  	printTime(); fprintf(logFile, " %s Exited book editor menu.\n", userLogin);
  	return;
  case '0':
    if (userClass == 0) {
      if (clearmode == 1) system("clear");
    menu(); return;}
    }
  editBook();
}

void editStudent(){
  if (checkExit == 1) return;
  if (userClass!=0) printf("Hello, %s!\n", userLogin);
  printf("What would you like to do?\n1 - Edit student\n2 - Add student\n3 - Print all students\n4 - Take a look at the student's info\n5 - Save\n6 - Check out what books the student has taken\n7 - Exit\n");
  if (userClass == 0) printf("0 - Back\n");
  char action, trash, *markBook;
  action = getchar();
  while ((trash = getchar()) != '\n');
  switch (action) {
    case '1':
      markBook = malloc(22);
      if (clearmode == 1) system("clear");
      printf("Enter markBook of a student.\n");
      fgets(markBook, 20, stdin); *(markBook+strlen(markBook)-1) = '\0';
      student* searchRes = NULL;
      if ((searchRes = findStudent(studentRoot, markBook)) == NULL) {error(4); break;}
      printTime(); fprintf(logFile, " %s Editing existent student %s.\n", userLogin, markBook);
      printf("Here's student info:\n");
      studInfo(markBook);
      editStudMenu(searchRes);
      free(markBook);
      if (clearmode == 1) system("clear");
      break;
  case '2':
    if (clearmode == 1) system("clear");
  	printTime(); fprintf(logFile, " %s Adding new student.\n", userLogin);
  	addStudCons();
    if (clearmode == 1) system("clear");
    break;
  case '3':
    if (clearmode == 1) system("clear");
     printStudents(studentRoot); printf("\n");
     break;
  case '4':
    markBook = malloc(22);
    printf("Enter markBook of a student.\n");
    fgets(markBook, 20, stdin); *(markBook+strlen(markBook)-1) = '\0';
    if (findStudent(studentRoot, markBook) == NULL) {error(4); break;}
    studInfo(markBook);
    free(markBook);
    break;
  case '5': saveStudents(studentsFile);
    printTime(); fprintf(logFile, " %s Saved to file.\n", userLogin);
  	printf("Saved.\n");
    break;
  case '6':
    markBook = malloc(22);
    if (clearmode == 1) system("clear");
    printf("Enter markbook of a student.\n");
    fgets(markBook, 20, stdin); *(markBook+strlen(markBook)-1) = '\0';
    printBooks(markBook);
    printTime(); fprintf(logFile, " %s Printed books of student %s.\n", userLogin, markBook);
    free(markBook);
    break;
  case '7':
    if (clearmode == 1) system("clear");
  	printTime(); fprintf(logFile, " %s Exited  students menu.\n", userLogin);
  	checkExit = 1; printf("Exiting...\n");
    return;
  case '0':
    if (userClass == 0) {
      if (clearmode == 1) system("clear");
    menu(); return;}
  }
  editStudent();
}

void editStudMenu(student* searchRes) {
  char action, trash, *markBook, *firstName, *secondName, *thirdName, *department, *speciality;
  printf("What would you like to edit?\n");
  printf("1 - Surname\n2 - Name\n3 - Fathername\n4 - Department\n5 - Speciality\n6 - Remove student\n0 - Back\n");
  firstName = malloc(52), secondName = malloc(52),
  thirdName = malloc(52), department = malloc(52), speciality = malloc(202);
   action = getchar();
   while ((trash = getchar()) != '\n');
  markBook = searchRes->markBook;
  switch (action) {
    case '1':
      printf("Enter new surname:\n");
      fgets(firstName, 50, stdin); *(firstName+strlen(firstName)-1) = '\0';
      searchRes->firstName = firstName;
      printTime(); fprintf(logFile, " %s Edited firstName, new name is %s.\n", userLogin, firstName);
      break;
    case '2':
      printf("Enter new name:\n");
      fgets(secondName, 50, stdin); *(secondName+strlen(secondName)-1) = '\0';
      searchRes->secondName = secondName;
      printTime(); fprintf(logFile, " %s Edited secondName, new name is %s.\n", userLogin, secondName);
      break;
    case '3':
      printf("Enter new patronymic:\n");
      fgets(thirdName, 50, stdin); *(thirdName+strlen(thirdName)-1) = '\0';
      searchRes->thirdName = thirdName;
      printTime(); fprintf(logFile, " %s Edited thirdName, new name is %s.\n", userLogin, thirdName);
      break;
    case '4':
      printf("Enter new department:\n");
      fgets(department, 50, stdin); *(department+strlen(department)-1) = '\0';
      searchRes->department = department;
      printTime(); fprintf(logFile, " %s Edited department, new is %s.\n", userLogin, department);
      break;
     case '5':
      printf("Enter new speciality:\n");
      fgets(speciality, 200, stdin); *(speciality+strlen(speciality)-1) = '\0';
      searchRes->speciality = speciality;
      printTime(); fprintf(logFile, " %s Edited speciality, new is %s.\n", userLogin, speciality);
      break;
    case '6':
      if (searchRes->next) {
      printf("This student haven't handed over some books. Aborting...\n");
      printTime(); fprintf(logFile,
        " %s This student (%s) haven't handed over some books. Aborting...\n",
        userLogin, markBook);}
      else {
      studentRoot = removeStudent(studentRoot, markBook);
      printf("Student was removed.\n");
      printTime(); fprintf(logFile, " %s Removed student %s.\n", userLogin, markBook);}
      break;
    case '0':
      printTime(); fprintf(logFile, " %s Returned to the parent menu.\n", userLogin);
      if (clearmode == 1) system("clear");
      editStudent();
      break;
    free(markBook);
    free(firstName);
    free(secondName);
    free(thirdName);
    free(department);
    free(speciality);
    editStudMenu(searchRes);
  }
}

void editBookMenu(book* searchRes){
  char action, trash, *isbn, *title, *author, charTotal[20], charAvailable[20];
  printf("What would you like to edit?\n");
  printf("1 - Author\n2 - Title\n3 - Total amount\n4 - Available amount\n5 - Remove book\n0 - Back\n");
  action = getchar();
  while ((trash = getchar()) != '\n');
  switch (action) {
    case '1':
      author = malloc(202);
      printf("Enter new author:\n");
      fgets(author, 200, stdin); *(author+strlen(author)-1) = '\0';
      searchRes->author = author;
      if (clearmode == 1) system("clear");
      printTime(); fprintf(logFile, " %s Edited author, new author is %s.\n", userLogin, author);
      if (clearmode == 1) system("clear");
      return;
    case '2':
      title = malloc(202);
      printf("Enter new title:\n");
      fgets(title, 200, stdin); *(title+strlen(title)-1) = '\0';
      searchRes->title = title;
      if (clearmode == 1) system("clear");
      printTime(); fprintf(logFile, " %s Edited title, new title is %s.\n", userLogin, title);
      return;
    case '3':
      printf("Enter new total amount:\n");
      fgets(charTotal, 18, stdin); *(charTotal+strlen(charTotal)-1) = '\0';
      if (atoi(charTotal) < (searchRes->totalBooks-searchRes->availableBooks)) {
        printf("You're trying to set total amount (%d) less that amount of taken books (%d). Aborting...\n",
          atoi(charTotal), searchRes->totalBooks-searchRes->availableBooks);
        printTime(); fprintf(logFile,
          " %s Trying to set total amount (%d) less that amount of taken books (%d). Aborting...\n",
        userLogin, searchRes->totalBooks, searchRes->totalBooks-searchRes->availableBooks);
        if (clearmode == 1) system("clear");
        return;}
      searchRes->totalBooks = atoi(charTotal);
      if (clearmode == 1) system("clear");
      return;
    case '4':
      printf("Enter new available amount:\n");
      fgets(charAvailable, 18, stdin); *(charAvailable+strlen(charAvailable)-1) = '\0';
      searchRes->availableBooks = atoi(charAvailable);
      if (clearmode == 1) system("clear"); return;
    case '5':
      isbn = searchRes->isbn;
      if (searchRes->next) {
      printf("Some copies of this book are not handed over to the library. Aborting...\n");
      printTime(); fprintf(logFile,
        " %s Some copies of this book (%s) are not handed over to the library. Aborting...\n",
        userLogin, isbn);}
      else { bookRoot = removeBook(bookRoot, isbn);
      printf("Book %s was removed.\n", isbn);
      printTime(); fprintf(logFile, " %s Removed book %s.\n", userLogin, isbn);}
      return;
    case '0':
    	printTime(); fprintf(logFile, " %s Returned to parent menu.\n", userLogin);
      if (clearmode == 1) system("clear");
    	editBook(); return;
    editBookMenu(searchRes);
  }
}

void addBookCons() {
  if (clearmode == 1) system("clear");
  char *isbn, *title, *author, charTotal[20], charAvailable[20];
  int total, available;
  isbn = malloc(22); title = malloc(202); author = malloc(202);
  book* searchRes = NULL;
  printf("Enter ISBN of a new book (or type \"exit\" to exit).\n");
  fgets(isbn, 20, stdin); *(isbn+strlen(isbn)-1) = '\0';
  if (strcmp(isbn, "exit") == 0) {
  if (clearmode == 1) system("clear");
  goto end;}
  if ((searchRes = findBook(bookRoot, isbn)) != NULL) {error(1); return;}
  printf("Enter title of the book.\n");
  fgets(title, 200, stdin); *(title+strlen(title)-1) = '\0';
  printf("Enter author of the book.\n");
  fgets(author, 200, stdin); *(author+strlen(author)-1) = '\0';
  printf("Enter total amount of books.\n");
  fgets(charTotal, 18, stdin); total = atoi(charTotal);
  printf("Enter available amount of books.\n");
  fgets(charAvailable, 18, stdin); available = atoi(charAvailable);
  if ((searchRes = addBook(bookRoot, isbn, title, author, total, available)) != NULL)
    printf("The book was successfully added.\n");
  else printf("Error occured while adding new book. Aborting...\n");
  return;
  end:
  free(isbn);
  free(author);
  free(title);
}

void addStudCons() {
  if (clearmode == 1) system("clear");
  char *markBook, *firstName, *secondName, *thirdName, *department, *speciality;
  markBook = malloc(22), firstName = malloc(52), secondName = malloc(52),
  thirdName = malloc(52), department = malloc(52), speciality = malloc(202);
  student* searchRes = NULL;
  printf("Enter markBook of a new student (or type \"exit\" to exit).\n");
  fgets(markBook, 20, stdin); *(markBook+strlen(markBook)-1) = '\0';
    if (strcmp(markBook, "exit") == 0) {
    if (clearmode == 1) system("clear");
    goto end;}
  if ((searchRes = findStudent(studentRoot, markBook)) != NULL) {error(2); return;}
  printf("Enter firstName of the student.\n");
  fgets(firstName, 50, stdin); *(firstName+strlen(firstName)-1) = '\0';
  printf("Enter secondName of the student.\n");
  fgets(secondName, 50, stdin); *(secondName+strlen(secondName)-1) = '\0';
  printf("Enter thirdName of the student.\n");
  fgets(thirdName, 50, stdin); *(thirdName+strlen(thirdName)-1) = '\0';
  printf("Enter department of the student.\n");
  fgets(department, 50, stdin); *(department+strlen(department)-1) = '\0';
  printf("Enter speciality of the student.\n");
  fgets(speciality, 200, stdin); *(speciality+strlen(speciality)-1) = '\0';
  if ((searchRes = addStudent(studentRoot, markBook, firstName,
    secondName, thirdName, department, speciality)) != NULL)
    printf("The student was successfully added.\n");
  else printf("Error occured while adding. Aborting...\n");
  return;
  end:
  free(markBook);
  free(firstName);
  free(secondName);
  free(thirdName);
  free(department);
  free(speciality);
}

void printTime() {
	time_t t = time(NULL);
	struct tm* aTm = localtime(&t);
	fprintf(logFile, "%04d/%02d/%02d %02d:%02d:%02d",
		aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday,
		aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

void error(int code) {
  switch (code) {
    case 1:
      printf("This book was already added to the library.\n");
      break;
    case 2:
      printf("This student was alredy registered in the library!\n");
      break;
    case 3:
      printf("The book was not found in the library.\n"); break;
    case 4:
      printf("The student was not found in the database.\n"); break;
    case 11:
      printf("Invalid arguments. Must be 3 args: books.csv, students.csv and users.csv.\nOnly in that order\n");
      exit(0);
      break;
  }
}

void man() {
  FILE *manual = fopen("manual.txt", "r");
  char c;
  while ((c = fgetc(manual)) != EOF) putchar(c);
  exit(0);
}

void clearenabled() {
  char c, trash=0;
  printf("Execute program without arguments to take a look at the manual.\n");
  printf("Would you like to clear the screen after every finished action?\n");
  printf("Type \"y\" to accept, anything else to refuse.\n");
  c = getchar();
  while (trash != '\n') trash = getchar();
  if (c == 'y') clearmode = 1;
  else clearmode = 0;
}
