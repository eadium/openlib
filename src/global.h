#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "books.h"
#include "students.h"
#include "taken.h"
#include "users.h"
#include "menu.h"

book *bookRoot;
student *studentRoot;
user *usersRoot;
int userClass;
char *booksFile, *studentsFile, *studentBooksFile;
FILE *logFile;
char* userLogin;
int checkExit;
int clearmode;
