#include "global.h"

int bfactorBook(book*);
book* rotateBookRight(book*);
book* rotateBookLeft(book*);
book* balanceBook(book*);
book* findminBook(book*);
book* removeminBook(book*);
void fixheightBook(book*);

unsigned char treeheight(book* node) {
    return node ? node->height : 0; }

int bfactorBook(book* node){
    return treeheight(node->right) - treeheight(node->left);}

void fixheightBook(book* node) {
    unsigned char leftHeight = treeheight(node->left);
    unsigned char rightHeight = treeheight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; }

book* rotateBookRight(book* node) {
    book* tempNode = node->left;
    node->left = tempNode->right;
    tempNode->right = node;
    fixheightBook(node);
    fixheightBook(tempNode);
    return tempNode; }

book* rotateBookLeft(book* node) {
    book* tempNode = node->right;
    node->right = tempNode->left;
    tempNode->left = node;
    fixheightBook(node);
    fixheightBook(tempNode);
    return tempNode; }

book* balanceBook(book* node) {
    fixheightBook(node);
    if (bfactorBook(node) == 2) {
        if (bfactorBook(node->right) < 0)
            node->right = rotateBookRight(node->right);
        return rotateBookLeft(node); }
    if (bfactorBook(node) == -2) {
        if (bfactorBook(node->left) > 0)
            node->left = rotateBookLeft(node->left);
        return rotateBookRight(node); }
    return node;
}

book *findBook(book *p, char *key) {
  if (!p) return NULL;
  int diff = strcmp(key, p->isbn);
  if (diff==0) return p;
  else if (diff < 0) findBook(p->left, key);
  else findBook(p->right, key);
}

book* addBook(book* node, char* isbn, char* author, char* title, int totalBooks, int availableBooks) {
    if (node == NULL) {
        node = (book*)malloc(sizeof(book));
        node->left = NULL;
        node->right = NULL;
        node->next = NULL;
        node->isbn = isbn;
        node->author = author;
        node->title = title;
        node->availableBooks = availableBooks;
        node->totalBooks = totalBooks;
        return node; }
    else {
        int cmp = strcmp(isbn, node->isbn);
        if (cmp == 0) {
            printf("Эта книга (ISBN %s) уже добавлена!\n", isbn); return NULL; }
         if (cmp < 0)
            node->left = addBook(node->left, isbn, author, title, totalBooks, availableBooks);
        else
            node->right = addBook(node->right, isbn, author, title, totalBooks, availableBooks); }
    return balanceBook(node);
}

void bookInfo(char* isbn) {
  book* tempBook = findBook(bookRoot, isbn);
  printf("%-s  %s  %s  %d  %d\n", tempBook->isbn, tempBook->author, tempBook->title,
  tempBook->totalBooks, tempBook->availableBooks);}

book* scanBooks(char *libName) {
  book *root = NULL, *newBook = NULL;
  FILE *lib = fopen(libName, "r");
  FILE *lib2 = fopen(libName, "r");
  while ((newBook = eatbook(lib, lib2)) != NULL){
      root = addBook(root, newBook->isbn, newBook->author, newBook->title, newBook->totalBooks, newBook->availableBooks);
      free(newBook); }
  fclose(lib); fclose(lib2);
  return root;
}

void printBook(book *p) {
    if (p != NULL) {
        printBook(p->left);
        printf("%-s  %s  %s  %d  %d\n", p->isbn, p->title, p->author, p->totalBooks, p->availableBooks);
        printBook(p->right); }
}

void freeBooksList(bookHolder* p) {
  if (p) {
    freeBooksList(p->next);
    free(p->date);
    free(p->markBook);
    free(p);}
  return;
}

void freeBook(book* node){
    if (!node) return;
    freeBook(node->left);
    freeBook(node->right);
    free(node->isbn);
    free(node->author);
    free(node->title);
    freeBooksList(node->next);
    free(node);
    node = NULL;}


book* eatbook(FILE *bookCount, FILE *bookRead) {
  book *newBook = (book*)malloc(sizeof(book));
  int cell=0;
  while (1) {
    int i=0, c = fgetc(bookCount);
    if (c==EOF) {free(newBook); return NULL;}
    while ((c != ';')&&(c != '\n')&&(c != EOF)&&(c != '\r')) {
      i++;
      if (c == '"') {
        while((c=fgetc(bookCount)) != '"') i++;
        i++; }
      c = fgetc(bookCount);
    }
  if (((c==EOF)||(c=='\n')||(c=='\r'))&&(cell!=4)) error(6);
    cell++;
    if (cell<4) {
      char* newCell = (char *)malloc(1+i*sizeof(char));
      for (int j=0; j < i; j++) *(newCell+j) = fgetc(bookRead);
      *(newCell+i) = '\0';
      if (cell==1) newBook->isbn = newCell;
      if (cell==2) newBook->title = newCell;
      if (cell==3) newBook->author = newCell;
    }
    else if (cell==4) fscanf(bookRead, "%d", &(newBook->totalBooks));
    else if (cell==5) fscanf(bookRead, "%d", &(newBook->availableBooks));
    c = fgetc(bookRead);
    if (c=='\r') {fgetc(bookRead); fgetc(bookCount);}
    if (cell == 5) {
      newBook->next = NULL;
      return newBook;}
  }
}

void saveBook(book* p, FILE* fileBooks, FILE* fileBooksStuds) {
  if (p) {
      saveBook(p->left, fileBooks, fileBooksStuds);
      fprintf(fileBooks, "%s;%s;%s;%d;%d\n", p->isbn, p->title, p->author,  p->totalBooks, p->availableBooks);
      bookHolder *next = p->next;
      while (next) {
        fprintf(fileBooksStuds, "%s;%s;%s\n", p->isbn, next->markBook, next->date);
        if (!next->next) break;
        else next = next->next; }
      saveBook(p->right, fileBooks, fileBooksStuds); }
}

void saveBooks(char* booksFileName, char* bookStudFileName) {
  FILE* file1 = fopen(booksFileName, "w");
  FILE* file2 = fopen(bookStudFileName, "w");
  saveBook(bookRoot, file1, file2);
  fclose(file1); fclose(file2);
}


book* findminBook(book* p) // поиск узла с минимальным ключом в дереве p
{
  return p->left?findminBook(p->left):p;
}

book* removeminBook(book* p) // удаление узла с минимальным ключом из дерева p
{
  if( p->left==0 )
    return p->right;
  p->left = removeminBook(p->left);
  return balanceBook(p);
}

book* removeBook(book* p, char* isbn) // удаление ключа k из дерева p
{
  if (!p) return NULL;
  int diff = strcmp(p->isbn, isbn);
  if (diff>0)
    p->left = removeBook(p->left,isbn);
  else if (diff<0)
    p->right = removeBook(p->right,isbn);
  else //  k == p->key
  {
    if (p->next) {
      printf("Some copies of this book are not holded back to the library. Aborting...\n");
      return NULL; }
    book* q = p->left;
    book* r = p->right;
    free(p->isbn);
    free(p->author);
    free(p->title);
    free(p->left);
    free(p->right);
    free(p);
    if(!r) return q;
    book* min = findminBook(r);
    min->right = removeminBook(r);
    min->left = q;
    return balanceBook(min);
  }
  return balanceBook(p);
}
