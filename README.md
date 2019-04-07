# Openlib

A simple & fast library management system written in pure C.

## Running on your machine

### Requirements
* GCC

### Step 1. Building

```
./make.sh
```

### Step 2. Running
The program uses CSV files to store data.
To run openlib, you need to launch it with 4 arguments:
```
./openlib test_data/books.csv test_data/students.csv test_data/users.csv test_data/student_books.csv
```

#### 1. books.csv

Contains all information about books in the whole library.

Valid format:

`ISBN;title;author(s);totalBooks;availableBooks`

#### 2. students.csv

Contains info about students using the library.

Valid format:

`ID_of_markbook;surname;name;patronymic;department;speciality`

#### 3. users.csv

Contains information about DBMS users.

Valid format:

`login;password;flag1;flag2`

`flag1` - could be 0 or 1, defines access to editing books;

`flag2` - could also be only 0 or 1, defines access to editing students.

#### 4. student_books.csv

Contains info about books that were taken by the students.

Valid format:

`ISBN;ID of markbook;date of return`

Please, pay your attention to the fact, that for correct work all the files are needed to be valid.

That means ISBN's, ID's of markbooks and amounts must be synchronized throughout the whole library.

Otherwise appropriate warnings will be shown.

In case an error is not fatal (e.g. unsynchronized amounts of books) nothing
terrible will happen - taken books simply won't be shown in student's profiles.

### Step 3. Using
***
All actions of users in the system are being written to the `log.txt` file.
***
To keep your data safely autosave is enabled. Moreover, you can save the current state forcedly through the menu.
***
Pressing "y" after starting the program will launch ClearMode.

In this mode the screen will be cleared after any completed action