# Openlib

A simple & fast library management system written in pure C.

## Running on your machine

### Requirements
* GCC

### Step 1. Building

```
git clone https://github.com/eadium/openlib.git
cd openlib
make
```

### Step 2. Running
Openlib uses CSV files to store data. To run openlib, you need to launch it with 4 arguments:
```
./openlib test_data/books.csv test_data/students.csv test_data/users.csv test_data/student_books.csv
```

File                              | Description                                 | Format
---                               | ---                                         | ---
books.csv                         | Info about books in the library.            | `ISBN;title;author(s);totalBooks;availableBooks`
students.csv                      | Info about students using the library.      | `ID_of_markbook;surname;name;patronymic;department;speciality`
users.csv                         | Info about DBMS users.                      | `login;password;flag1;flag2`
student_books.csv                 | Info about books that have been loaned by the students. | `ISBN;ID of markbook;date of return`

* `flag1` - 0 or 1, defines access to editing books
* `flag2` - 0 or 1, defines access to editing students

In order to assure correct work from the application, make sure that all .csv files are valid. This means ISBNs, IDs of markbooks and amounts must be synchronized throughout the whole library. Otherwise you will be warned. In case of badly synchronized files (e.g. unsynchronized amounts of books) nothing terrible will happen - loaned books simply won't be shown in students' profiles.

### Step 3. Using
* All actions of users in the system are being written to the `log.txt` file.
* To keep your data safe, autosave is enabled by default. Moreover, you can save the current state by choosing <b>Save</b> in the menu of the application.
* Pressing "y" after starting the program will launch ClearMode.