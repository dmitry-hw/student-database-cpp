#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

struct StudentRecord {
    char fullName[60];      // Last name and initials (e.g., "Smith J.D.")
    int birthYear;          // Birth year (1950..2010)
    char group[15];         // Academic group number
    int physics;            // Physics grade (1..10)
    int math;               // Mathematics grade (1..10)
    int compScience;        // Computer Science grade (1..10)
    int chemistry;          // Chemistry grade (1..10)
    double gpa;             // Grade Point Average
};

void printRecord(StudentRecord z, FILE *Ft);
void printHeader(FILE *Ft);
void inputRecord(StudentRecord *z);
int checkStartLetter(const char* fio);

// Input validation functions
int inputInt(const char* prompt, int minVal, int maxVal);
void inputFullName(const char* prompt, char* dest, int maxLen);
void inputGroup(const char* prompt, char* dest);

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    int choice, subChoice, confirm;
    StudentRecord currentRecord;
    char targetGroup[15];
    const char* dbFile = "students.dat";
    const char* reportFile = "report.txt";
    int size = sizeof(StudentRecord);

    FILE *Ft = fopen(reportFile, "w");
    if (Ft == NULL) {
        printf("\nError: Could not create the report file!\n");
        return 1;
    }

    while (1) {
        printf("\n================ MENU ================\n");
        printf(" 1 - Create Database (Create)\n");
        printf(" 2 - View Records (View)\n");
        printf(" 3 - Modify (Add or Edit Record)\n");
        printf(" 4 - Search by Group & Last Name (A, B, C)\n");
        printf(" 5 - Sort by GPA\n");
        printf(" 0 - EXIT\n");
        printf("======================================");

        choice = inputInt("\nSelect a menu option (0..5): ", 0, 5);

        switch (choice) {
        case 1: {
            FILE *Fz = fopen(dbFile, "rb");
            if (Fz != NULL) {
                fseek(Fz, 0, SEEK_END);
                long len = ftell(Fz);
                fclose(Fz);
                if (len > 0) {
                    printf("\n WARNING: Database file already exists and contains data!\n");
                    confirm = inputInt(" Overwrite file and delete old records? (1 - Yes, 0 - No): ", 0, 1);
                    if (confirm == 0) {
                        printf(" Creation canceled. Old data preserved.\n");
                        break;
                    }
                }
            }

            Fz = fopen(dbFile, "wb");
            if (Fz == NULL) {
                printf("\nError: Could not create binary database file!\n");
                break;
            }
            fclose(Fz);
            printf("\nNew empty file %s created!\n", dbFile);
            fprintf(Ft, "\n--- Created new file %s ---\n", dbFile);
            break;
        }

        case 2: {
            FILE *Fz = fopen(dbFile, "rb");
            if (Fz == NULL) {
                printf("\nFile not found! Please create it first (Option 1).\n");
                break;
            }

            printf("\n\t--------- Student Database Records ---------\n");
            fprintf(Ft, "\n\t--------- Student Database Records ---------\n");
            printHeader(Ft);

            while (fread(&currentRecord, size, 1, Fz) == 1) {
                printRecord(currentRecord, Ft);
            }
            fclose(Fz);
            break;
        }

        case 3: {
            printf("\n --- MODIFY ---\n");
            printf(" 1 - Add a new record to the end of the file\n");
            printf(" 2 - Edit an existing record by number\n");
            printf(" 0 - Cancel\n");
            subChoice = inputInt(" Your choice (0..2): ", 0, 2);

            if (subChoice == 0) break;
            if (subChoice == 1) {
                FILE *Fz = fopen(dbFile, "ab");
                if (Fz == NULL) {
                    printf("\nError opening file for appending!\n");
                    break;
                }
                inputRecord(&currentRecord);
                fwrite(&currentRecord, size, 1, Fz);
                fclose(Fz);
                printf("Record successfully added to the file!\n");
            }
            else if (subChoice == 2) {
                FILE *Fz = fopen(dbFile, "r+b");
                if (Fz == NULL) {
                    printf("\nFile not found! Please create it first (Option 1).\n");
                    break;
                }

                fseek(Fz, 0, SEEK_END);
                long len = ftell(Fz);
                int total = (int)(len / size);

                if (total == 0) {
                    printf("File is empty, nothing to edit!\n");
                    fclose(Fz);
                    break;
                }

                printf("\nTotal records in file: %d.\n", total);
                int index = inputInt("Enter record number to edit: ", 1, total);

                printf("\nEnter new data for student #%d:\n", index);
                inputRecord(&currentRecord);

                fseek(Fz, (long)(index - 1) * size, SEEK_SET);
                fwrite(&currentRecord, size, 1, Fz);
                fclose(Fz);
                printf("Record successfully updated!\n");
            }
            break;
        }

        case 4: {
            FILE *Fz = fopen(dbFile, "rb");
            if (Fz == NULL) {
                printf("\nFile not found! Please create it first (Option 1).\n");
                break;
            }

            inputGroup("\nEnter target group number: ", targetGroup);

            printf("\n\t--- SEARCH RESULT (Group: %s, Last names starting with A, B, C) ---\n", targetGroup);
            fprintf(Ft, "\n\t--- SEARCH RESULT (Group: %s, Last names starting with A, B, C) ---\n", targetGroup);
            printHeader(Ft);

            int foundCount = 0;
            while (fread(&currentRecord, size, 1, Fz) == 1) {
                if (strcmp(currentRecord.group, targetGroup) == 0 && checkStartLetter(currentRecord.fullName)) {
                    printRecord(currentRecord, Ft);
                    foundCount++;
                }
            }
            fclose(Fz);

            if (foundCount == 0) {
                printf("No students matching the criteria were found.\n");
                fprintf(Ft, "No students matching the criteria were found.\n");
            }
            break;
        }

        case 5: {
            FILE *Fz = fopen(dbFile, "rb");
            if (Fz == NULL) {
                printf("\nFile not found! Please create it first (Option 1).\n");
                break;
            }

            fseek(Fz, 0, SEEK_END);
            long len = ftell(Fz);
            int total = (int)(len / size);
            rewind(Fz);

            if (total == 0) {
                printf("File is empty! Nothing to sort.\n");
                fclose(Fz);
                break;
            }

            StudentRecord* records = new StudentRecord[total];
            for (int i = 0; i < total; i++) {
                fread(&records[i], size, 1, Fz);
            }
            fclose(Fz);

            // Simple exchange sort (ascending by GPA)
            for (int i = 0; i < total - 1; i++) {
                for (int j = i + 1; j < total; j++) {
                    if (records[i].gpa > records[j].gpa) {
                        StudentRecord temp = records[i];
                        records[i] = records[j];
                        records[j] = temp;
                    }
                }
            }

            printf("\n\t--------- SORTED (Ascending by GPA) ---------\n");
            fprintf(Ft, "\n\t--------- SORTED (Ascending by GPA) ---------\n");
            printHeader(Ft);

            for (int i = 0; i < total; i++) {
                printRecord(records[i], Ft);
            }

            delete[] records;
            break;
        }

        case 0:
            fclose(Ft);
            printf("\nProgram finished. All results saved to %s\n", reportFile);
            return 0;
        }
    }
}

int inputInt(const char* prompt, int minVal, int maxVal) {
    char buf[256];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(buf, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf(" [Error]: Input is too long! Please try again.\n");
            continue;
        }

        buf[strcspn(buf, "\n")] = '\0';

        int i = 0;
        while (buf[i] == ' ' || buf[i] == '\t') i++;

        if (buf[i] == '\0') {
            printf(" [Error]: Empty input! Please enter a number.\n");
            continue;
        }

        int start = i;
        if (buf[i] == '-' || buf[i] == '+') i++;
        if (buf[i] == '\0') {
            printf(" [Error]: Enter digits, not just a sign!\n");
            continue;
        }

        int valid = 1;
        int digitCount = 0;
        for (; buf[i] != '\0'; i++) {
            if (buf[i] < '0' || buf[i] > '9') {
                valid = 0;
                break;
            }
            digitCount++;
        }

        if (!valid) {
            printf(" [Error]: Invalid characters! Enter an integer.\n");
            continue;
        }

        if (digitCount > 9) {
            printf(" [Error]: Number is too large! Range: %d to %d.\n", minVal, maxVal);
            continue;
        }

        long val = strtol(buf + start, NULL, 10);
        if (val < minVal || val > maxVal) {
            printf(" [Error]: Number out of range! Enter a value from %d to %d.\n", minVal, maxVal);
            continue;
        }

        return (int)val;
    }
}

void inputFullName(const char* prompt, char* dest, int maxLen) {
    char buf[256];
    while (1) {
        printf("%s", prompt);
if (fgets(buf, sizeof(buf), stdin) == NULL) {
clearerr(stdin);
continue;
}
if (strchr(buf, '\n') == NULL) {
int ch;
while ((ch = getchar()) != '\n' && ch != EOF);
printf(" [Error]: String is too long!\n");
continue;
}
buf[strcspn(buf, "\n")] = '\0';
int start = 0;
while (buf[start] == ' ' || buf[start] == '\t') start++;
int len = (int)strlen(buf + start);
if (len < 2) {
printf(" [Error]: Name is too short! (e.g., Smith J.D.).\n");
continue;
}
if (len >= maxLen) {
printf(" [Error]: Name is too long!\n");
continue;
}
unsigned char first = (unsigned char)buf[start];
int firstIsLetter = (first >= 'A' && first <= 'Z') || (first >= 'a' && first <= 'z') || (first >= 128);
if (!firstIsLetter) {
printf(" [Error]: Last name must start with a letter!\n");
continue;
}
int valid = 1;
for (int i = start; buf[i] != '\0'; i++) {
unsigned char c = (unsigned char)buf[i];
int isUtf8 = (c >= 128);
int isEng = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
int isAllowedPunct = (c == ' ' || c == '.' || c == '-');
if (!isUtf8 && !isEng && !isAllowedPunct) {
valid = 0;
break;
}
}
if (!valid) {
printf(" [Error]: Digits and special characters are not allowed in names!\n");
continue;
}
strcpy(dest, buf + start);
return;
}
}
void inputGroup(const char* prompt, char* dest) {
int grpNum = inputInt(prompt, 1, 1000000);
sprintf(dest, "%d", grpNum);
}
void inputRecord(StudentRecord *z) {
printf("\n --- Enter Student Details ---\n");
inputFullName(" Last Name & Initials (e.g., Smith J.D.): ", (*z).fullName, sizeof((*z).fullName));
(*z).birthYear = inputInt(" Birth Year (1950..2010): ", 1950, 2010);
inputGroup(" Group Number (1..1000000): ", (*z).group);
printf(" --- Enter Semester Grades (1 to 10) ---\n");
(*z).physics = inputInt("   Physics (1..10): ", 1, 10);
(*z).math = inputInt("   Mathematics (1..10): ", 1, 10);
(*z).compScience = inputInt("   Computer Science (1..10): ", 1, 10);
(*z).chemistry = inputInt("   Chemistry (1..10): ", 1, 10);
(*z).gpa = ((*z).physics + (*z).math + (*z).compScience + (*z).chemistry) / 4.0;
}
void printHeader(FILE *Ft) {
printf("---------------------------------------------------------------------------\n");
printf("| %-20s | %-4s | %-8s | %-3s | %-3s | %-3s | %-3s | %-6s |\n",
"Full Name", "Year", "Group", "Phy", "Mat", "CS", "Chm", "GPA");
printf("---------------------------------------------------------------------------\n");
fprintf(Ft, "---------------------------------------------------------------------------\n");
fprintf(Ft, "| %-20s | %-4s | %-8s | %-3s | %-3s | %-3s | %-3s | %-6s |\n",
"Full Name", "Year", "Group", "Phy", "Mat", "CS", "Chm", "GPA");
fprintf(Ft, "---------------------------------------------------------------------------\n");
}
void printRecord(StudentRecord z, FILE *Ft) {
printf("| %-20s | %-4d | %-8s | %-3d | %-3d | %-3d | %-3d | %6.3lf |\n",
z.fullName, z.birthYear, z.group, z.physics, z.math, z.compScience, z.chemistry, z.gpa);
fprintf(Ft, "| %-20s | %-4d | %-8s | %-3d | %-3d | %-3d | %-3d | %6.3lf |\n",
z.fullName, z.birthYear, z.group, z.physics, z.math, z.compScience, z.chemistry, z.gpa);
}
int checkStartLetter(const char* fio) {
// Checks if last name starts with A, B, or C (case-insensitive)
if (fio[0] == 'A' || fio[0] == 'a' ||
fio[0] == 'B' || fio[0] == 'b' ||
fio[0] == 'C' || fio[0] == 'c') {
return 1;
}
return 0;
}
