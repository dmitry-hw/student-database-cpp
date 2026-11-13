# Student Academic Performance Database (C++ Console App)

A cross-platform console application for managing a student database, storing records in a binary file, and automatically generating a text report.

## Features

* **Database Creation:** Initializes a new binary file (`zapisi.dat`) with protection against accidental overwriting of existing data.
* **View Records:** Displays all students and their grades as a formatted table in both the console and the report file (`rezult.txt`).
* **Data Editing:** Appends new student records to the end of the file or edits an existing record by its index number.
* **Filtering (Task Variant 6):** Searches for students in a specified academic group whose last names start with the Cyrillic letters **В**, **Г**, or **Д** (or their Latin equivalents).
* **Sorting:** Sorts the student list in ascending order by their Grade Point Average (GPA).
* **Input Validation:** Protects against invalid numeric inputs, birth years, full name formats, and group numbers.

## Data Structure

Each student record (`struct TZap`) stores:
* **Full Name** (Last name and initials)
* **Birth Year** (range 1950–2010)
* **Group Number**
* **Grades** in four subjects (Physics, Mathematics, Computer Science, Chemistry)
* **Average Grade / GPA** (calculated automatically)

## Requirements & Running the App

The program is cross-platform and compiles on **Windows, Linux, and macOS** using any standard C++ compiler (GCC, Clang, MSVC).

### Compile and Run (GCC / Clang):
```bash
g++ main.cpp -o students_db
./students_db
```
*(On Windows, run `students_db.exe`)*

### Run in Visual Studio (Windows):
1. Create an empty C++ project.
2. Add the `main.cpp` file (ensure it is saved with **UTF-8** encoding).
3. Build and run the project (`Ctrl + F5`).

## Generated Files

During execution, the program automatically creates two files in its working directory:
* `zapisi.dat` — binary file used to store the database.
* `rezult.txt` — plain text log of all performed operations and search results.
