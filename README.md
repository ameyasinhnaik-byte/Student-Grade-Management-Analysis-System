# Student Grade Management & Analysis System

## Overview
A command-line application written in **C** to manage and analyze student academic records using CSV datasets.  
The system processes student marks, computes grades, and performs academic analysis such as SGPA, CGPA, toppers, and statistical metrics.

This project demonstrates concepts from **Data Structures, File Handling, and Systems Programming in C**.

---

## Features

- Calculate **grade of a student in a specific subject**
- Display **grades of all students**
- Compute **SGPA for a given semester**
- Compute **CGPA across semesters**
- Identify **subjects failed by a student**
- Find **topper of a subject**
- Find **top N students in a subject**
- Show **semester toppers**
- Compute **standard deviation of marks for a subject**

---

## Supported Commands

| Command | Description |
|--------|-------------|
| `grade MIS SUBJECT` | Returns grade of a student in a subject |
| `grade all` | Prints grades of all students |
| `sgpa MIS SEM` | Calculates SGPA for a given semester |
| `cgpa MIS` | Calculates CGPA |
| `failed MIS` | Lists subjects failed by a student |
| `topsem SEM` | Shows topper for each subject in a semester |
| `topsub SUBJECT` | Shows topper of a subject |
| `topnsub SUBJECT N` | Shows top N students in a subject |
| `stdev SUBJECT` | Computes standard deviation of marks |

### Example
grade 612303001 dsa
sgpa 612303001 2
cgpa 612303001
failed 612303001
topsub dsa
topnsub dsa 5
stdev dsa

---

## Dataset Structure

The program uses three CSV files.

### subjects.csv

Contains subject information -> 
subject_name,credits,semester

---

### grades.csv

Contains grade boundaries used for grade classification.

Grades include: FF, DD, CD, CC, BC, BB, AB, AA

---

### marks.csv

Contains student marks for subjects.

Example: MIS,sub1,sub2,sub3,...

---

## Technical Implementation

### Data Structures
- Custom **structs** for subjects, grades, and marks
- Arrays to store dataset information
- Dynamic memory allocation for failed subject lists

### Algorithms Used
- **Insertion Sort** for grade boundary ordering
- **Statistical calculations** for mean and standard deviation
- **Linear search** for MIS lookup

### System Programming Concepts
Low-level file operations are performed using system calls:
`open()`
`read()`
`close()`
instead of standard `fopen`.

### Memory Management
- Dynamic allocation using `malloc`
- Memory cleanup using `free`

---

## Compilation
gcc tables.c -o grade -lm
OR
use the `make` command

## Running the program
./grade

Then enter commands in the CLI as shown in commands.txt and exit using `exit` command


---

## Concepts Demonstrated

- File handling using system calls
- Structured data modeling using `struct`
- CSV parsing
- Academic grading logic
- Statistical computation
- Command-line application design

---

## Future Improvements

- Improved command parsing
- Support for larger datasets
- Export analysis results to CSV
- Faster search using hash tables
