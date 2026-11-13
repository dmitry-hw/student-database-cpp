# Student Academic Performance Database (C++ CLI)

A cross-platform console application written in C++ for managing student academic records using binary file storage and automated text report generation.

## Features

* **Database Initialization:** Creates a new binary file (`students.dat`) with overwrite protection to prevent accidental loss of existing records.
* **Tabular View:** Displays all student records and grades as a formatted table in both the console and the log file (`report.txt`).
* **Record Modification:** Appends new student entries to the end of the file or updates specific records in-place using binary file offsets (`fseek`).
* **Group & Initial Search:** Filters students belonging to a specified academic group whose last names start with **A**, **B**, or **C**.
* **GPA Sorting:** Dynamically loads records into memory and sorts them in ascending order by Grade Point Average (GPA).
* **Input Validation:** Protects against buffer overflows, invalid numeric types, out-of-range values, and malformed names.

## Data Structure

Each `StudentRecord` structure stores:
* **Full Name** (`char fullName[60]`) — Last name and initials (e.g., `Smith J.D.`)
* **Birth Year** (`int`) — Validated range `1950–2010`
* **Group Number** (`char group[15]`) — Academic group identifier
* **Subject Grades** (`int`) — Physics, Mathematics, Computer Science, and Chemistry (`1–10` scale)
* **GPA** (`double`) — Automatically calculated average grade

## Build & Run

The program has no external dependencies and compiles cleanly on **Windows, Linux, and macOS**.

### Using GCC / Clang:
```bash
g++ main.cpp -o students_db
./students_db
```
*(On Windows, run `students_db.exe`)*

### Using Visual Studio (Windows):
1. Create an empty C++ Console Project.
2. Add `main.cpp` to the source files.
3. Build and run (`Ctrl + F5`).

## Generated Files

During execution, the program creates two files in its working directory:
* `students.dat` — binary database file storing `StudentRecord` structures.
* `report.txt` — plain text report logging all tables and search results from the session.
