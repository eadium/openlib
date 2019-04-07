#include "global.h"

int main(int argc, char * argv[]) {
    if (argc != 5) man();
    clearenabled();
    logFile = fopen("log.txt", "a");
    setlocale(LC_ALL, "Rus");
    userLogin = "Guest";
    fprintf(logFile, "\n");
    printTime(); fprintf(logFile, " Started program.\n");
    booksFile = argv[1];
    studentsFile = argv[2];

    bookRoot = scanBooks(booksFile);
    studentRoot = scanStudents(studentsFile);
    usersRoot = scanUsers(argv[3]);
    studentBooksFile = argv[4];
    scantakenBooks(argv[4]);
    checkExit = 0;
    auth();
    saveBooks("book_Save.csv", "studentBooks_Save.csv");
    saveStudents("student_Save.csv");
    freeBook(bookRoot);
    freeStudents(studentRoot);
    freeUsers(usersRoot);
    fclose(logFile);
}
