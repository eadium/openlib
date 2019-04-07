#include "global.h"

int bfactor(student*);
student* rotateRight(student*);
student* rotateLeft(student*);
student* balance(student*);
student* findmin(student*);
student* removemin(student*);

unsigned char treeheightStudents(student* node) {
    return node ? node->height : 0; }

int bfactor(student* node){
    return treeheightStudents(node->right) - treeheightStudents(node->left);}

void fixheightStud(student* node) {
    unsigned char leftHeight = treeheightStudents(node->left);
    unsigned char rightHeight = treeheightStudents(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; }

student* rotateRight(student* node) {
    student* tempNode = node->left;
    node->left = tempNode->right;
    tempNode->right = node;
    fixheightStud(node);
    fixheightStud(tempNode);
    return tempNode; }

student* rotateLeft(student* node) {
    student* tempNode = node->right;
    node->right = tempNode->left;
    tempNode->left = node;
    fixheightStud(node);
    fixheightStud(tempNode);
    return tempNode; }

student* balance(student* node) {
    fixheightStud(node);
    if (bfactor(node) == 2) {
        if (bfactor(node->right) < 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node); }
    if (bfactor(node) == -2) {
        if (bfactor(node->left) > 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node); }
    return node;}

void saveStudent(student* p, FILE* file) {
  if (p != NULL) {
      saveStudent(p->left, file);
      fprintf(file, "%s;%s;%s;%s;%s;%s\n", p->markBook, p->firstName, p->secondName, p->thirdName,  p->department, p->speciality);
      saveStudent(p->right, file); }
}

void studInfo(char* markBook) {
  student* tempStud = findStudent(studentRoot, markBook);
  printf("%-s  %s  %s  %s  %s  %s\n", tempStud->markBook, tempStud->firstName,
  tempStud->secondName, tempStud->thirdName, tempStud->department, tempStud->speciality); }

void saveStudents(char* fileName) {
  FILE* file = fopen(fileName, "w");
  saveStudent(studentRoot, file);
  fclose(file);
}

student *findStudent(student *p, char *key) {
  if (!p) return NULL;
  int diff = strcmp(key, p->markBook);
  if (diff==0) return p;
  else if (diff < 0) findStudent(p->left, key);
  else findStudent(p->right, key); }

student* addStudent(student* node, char* markBook, char* firstName, char* secondName, char* thirdName, char* department, char* speciality) {
    if (node == NULL) {
        node = (student*)malloc(sizeof(student)+1);
        node->left = NULL;
        node->right = NULL;
        node->next = NULL;
        node->markBook = markBook;
        node->firstName = firstName;
        node->secondName = secondName;
        node->thirdName = thirdName;
        node->department = department;
        node->speciality = speciality;
        return node; }
    else {
        int cmp = strcmp(markBook, node->markBook);
        if (cmp == 0) {
            printf("Этот студент (зачётка %s) уже есть в базе данных!\n", markBook); return NULL; }
         if (cmp < 0)
            node->left = addStudent(node->left, markBook, firstName, secondName, thirdName, department, speciality);
        else
            node->right = addStudent(node->right, markBook, firstName, secondName, thirdName, department, speciality); }
    return balance(node);
}

student* scanStudents(char *listName) {
  student *root = NULL, *newStudent = NULL;
  FILE *list = fopen(listName, "r"), *list2 = fopen(listName, "r");
  while ((newStudent = eatstudent(list, list2)) != NULL) {
      root = addStudent(root, newStudent->markBook, newStudent->firstName, newStudent->secondName, newStudent->thirdName, newStudent->department, newStudent->speciality);
      free(newStudent);
  }
  fclose(list); fclose(list2);
  return root;
}

student* eatstudent(FILE *studCount, FILE *studRead) {
  student *newStudent = (student*)malloc(sizeof(student));
  int cell=0;
  while (1) {
    int i=0, c = fgetc(studCount);
    if (c==EOF) {free(newStudent); return NULL;}
    while ((c != ';')&&(c != '\n')&&(c != EOF)&&(c != '\r')) {
      i++;
      if (c == '"') {
        while((c=fgetc(studCount)) != '"') i++;
        i++; }
      c = fgetc(studCount);
    }
    if (((c==EOF)||(c=='\n')||(c=='\r'))&&(cell!=5)) error(6);
    cell++;
    char* newCell = (char *)malloc(1+i*sizeof(char));
    for (int j=0; j < i; j++) *(newCell+j) = fgetc(studRead);
    *(newCell+i) = '\0';
    switch (cell) {
      case 1: newStudent->markBook = newCell;    break;
      case 2: newStudent->firstName = newCell;   break;
      case 3: newStudent->secondName = newCell;  break;
      case 4: newStudent->thirdName = newCell;   break;
      case 5: newStudent->department = newCell;  break;
      case 6: newStudent->speciality = newCell;  break;
    }
    c = fgetc(studRead);
    if (c=='\r') {fgetc(studRead); fgetc(studCount);}
    if (cell == 6) {
      newStudent->next = NULL;
      return newStudent; }
  }
}

void printStudents(student *student) {
    if (student != NULL) {
        printStudents(student->left);
        printf("%-s  %s  %s  %s  %s  %s\n", student->markBook, student->firstName,
        student->secondName, student->thirdName, student->department, student->speciality);
        printStudents(student->right);}
}

void freeStudList(studentBooks* p) {
  if (p) {
    freeStudList(p->next);
    free(p->date);
    free(p->isbn);
    free(p);}
  return;
}

void freeStudents(student* node){
    if (node == NULL) return;
    freeStudents(node->left);
    freeStudents(node->right);
    free(node->markBook);
    free(node->firstName);
    free(node->secondName);
    free(node->thirdName);
    free(node->department);
    free(node->speciality);
    freeStudList(node->next);
    free(node);
    node = NULL;}


student* findmin(student* p) // поиск узла с минимальным ключом в дереве p
{
  return p->left?findmin(p->left):p;
}

student* removemin(student* p) // удаление узла с минимальным ключом из дерева p
{
  if( p->left==0 )
    return p->right;
  p->left = removemin(p->left);
  return balance(p);
}

student* removeStudent(student* p, char* markBook) // удаление ключа из дерева p
{
  if (!p) return NULL;
  if ((strcmp(markBook, p->markBook)<0))
    p->left = removeStudent(p->left,markBook);
  else if (strcmp(markBook, p->markBook)>0)
    p->right = removeStudent(p->right,markBook);
  else
  {
    if (p->next) {
      printf("This student haven't handed over some books. Cannot remove. Aborting...\n");
      return NULL; }
    student* q = p->left;
    student* r = p->right;
    free(p->markBook);
    free(p->firstName);
    free(p->secondName);
    free(p->thirdName);
    free(p->department);
    free(p->speciality);
    free(p->left);
    free(p->right);
    free(p);
    if(!r) return q;
    student* min = findmin(r);
    min->right = removemin(r);
    min->left = q;
    return balance(min);
  }
  return balance(p);
}
