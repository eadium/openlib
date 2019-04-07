#pragma once

typedef struct user {
    char* login;
    char* password;
    int booksAccess;
    int studentAccess;
    unsigned char height;
    struct user* left;
    struct user* right;
} user;

user* eatuser(FILE*, FILE*);
user* scanUsers(char*);
user* addUser(user*, char*, char*, int, int);
void printUsers(user*);
user* findUser(user*, char*);
void freeUsers();
