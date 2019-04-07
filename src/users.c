#include "global.h"

int userNo=0;

int bfactorUser(user*);
user* rotateUserRight(user*);
user* rotateUserLeft(user*);
user* balanceUser(user*);
void fixheightUser(user*);

unsigned char treeheightUsers(user* node) {
    return node ? node->height : 0; }

int bfactorUser(user* node){
    return treeheightUsers(node->right) - treeheightUsers(node->left);}

void fixheightUser(user* node) {
    unsigned char leftHeight = treeheightUsers(node->left);
    unsigned char rightHeight = treeheightUsers(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; }

user* rotateUserRight(user* node) {
    user* tempNode = node->left;
    node->left = tempNode->right;
    tempNode->right = node;
    fixheightUser(node);
    fixheightUser(tempNode);
    return tempNode; }

user* rotateUserLeft(user* node) {
    user* tempNode = node->right;
    node->right = tempNode->left;
    tempNode->left = node;
    fixheightUser(node);
    fixheightUser(tempNode);
    return tempNode; }

user* balanceUser(user* node) {
    fixheightUser(node);
    if (bfactorUser(node) == 2) {
        if (bfactorUser(node->right) < 0)
            node->right = rotateUserRight(node->right);
        return rotateUserLeft(node); }
    if (bfactorUser(node) == -2) {
        if (bfactorUser(node->left) > 0)
            node->left = rotateUserLeft(node->left);
        return rotateUserRight(node); }
    return node;
}

user *findUser(user *p, char *key) {
  if (!p) return NULL;
  int diff = strcmp(key, p->login);
  if (diff==0) return p;
  else if (diff < 0) findUser(p->left, key);
  else findUser(p->right, key);
}

user* addUser(user* node, char* login, char* password, int booksAccess, int studentAccess) {
    if (node == NULL) {
        node = (user*)malloc(sizeof(user));
        node->left = NULL;
        node->right = NULL;
        node->login = login;
        node->password = password;
        node->studentAccess = studentAccess;
        node->booksAccess = booksAccess;
        return node; }
    else {
        int cmp = strcmp(login, node->login);
        if (cmp == 0) return NULL;
         if (cmp < 0)
            node->left = addUser(node->left, login, password, booksAccess, studentAccess);
        else
            node->right = addUser(node->right, login, password, booksAccess, studentAccess); }
    return balanceUser(node);
}

user* scanUsers(char *dbName) {
  user *root = NULL, *newUser;
  FILE *db = fopen(dbName, "r");
  FILE *db2 = fopen(dbName, "r");
  while ((newUser = eatuser(db, db2)) != NULL){
      root = addUser(root, newUser->login, newUser->password,  newUser->studentAccess, newUser->booksAccess);
      free(newUser);}
  fclose(db); fclose(db2);
  return root;
}

void printUsers(user *p) {
    if (p != NULL) {
        printUsers(p->left);
        printf("%-s  %s  %d  %d\n", p->login, p->password,  p->studentAccess,  p->booksAccess);
        printUsers(p->right); }
}

user* eatuser(FILE *fileEat, FILE *fileRead) {
    int i=0, c=0, length=0;
    char *login, *password;
    user *newUser;
    newUser = malloc(sizeof(user));
    userNo++;
      //читаем login
        while ((c = fgetc(fileRead)) != ';') {
          if (c == EOF) {free(newUser); return NULL;}
          length++; }
        login = malloc(length+2);
        for (i = 0; i <= length; i++) {
            *(login+i) = fgetc(fileEat);
            if (*(login+i) == ';') {
                *(login+i) = '\0';
                break; }
            }
        newUser->login = login;

        //читаем пароль
        length=0;
        while ((c = fgetc(fileRead)) != ';') {
            length++; }
        password = malloc(length+2);
        for (i = 0; i <= length; i++) {
            *(password+i) = fgetc(fileEat);
            if (*(password+i) == ';') {
                *(password+i) = '\0';
                break; }
            }
        newUser->password = password;

        //читаем право доступа к книгам
        if ((c = fgetc(fileEat)) == '1')
        newUser->booksAccess = 1;
        else newUser->booksAccess = 0;

        //читаем право доступа к студентам
        c = fgetc(fileEat);
        if ((c = fgetc(fileEat)) == '1')
        newUser->studentAccess = 1;
        else newUser->studentAccess = 0;

        c = fgetc(fileEat); c = fgetc(fileRead);
        c = fgetc(fileRead); c = fgetc(fileRead);
    return newUser;
}

void freeUsers(user* node){
    if (node == NULL) return;
    freeUsers(node->left);
    freeUsers(node->right);
    free(node->login);
    free(node->password);
    free(node);
    node = NULL;}
