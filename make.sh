#!/bin/bash
clear
gcc -c src/main.c
gcc -c src/books.c
gcc -c src/students.c
gcc -c src/users.c
gcc -c src/taken.c
gcc -c src/menu.c
echo "Compiled objective files"
gcc -o openlib books.o students.o users.o menu.o taken.o main.o
echo "Compilation finished"
rm *.o
