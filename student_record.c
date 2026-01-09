/*
----------------------------------------------------
 Student Record Management System
 Language : C
 Author   : Soham Patil
 Level    : Professional / GitHub Ready
----------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ================= CONSTANTS ================= */
#define FILE_NAME "students.dat"
#define PASS_MARKS 40

/* ================= STRUCTURE ================= */
typedef struct {
    int roll;
    char name[50];
    float marks;
} Student;

/* ================= FUNCTION PROTOTYPES ================= */
char getGrade(float);
int fileExists();
int containsIgnoreCase(const char *, const char *);

void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();
void searchStudent();
void classStatistics();
void sortStudents();

/* ================= UTILITIES ================= */
char getGrade(float marks) {
    if (marks >= 75) return 'A';
    if (marks >= 60) return 'B';
    if (marks >= PASS_MARKS) return 'C';
    return 'F';
}

int fileExists() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;
    fclose(fp);
    return 1;
}

int containsIgnoreCase(const char *str, const char *key) {
    char s1[50], s2[50];
    strcpy(s1, str);
    strcpy(s2, key);

    for (int i = 0; s1[i]; i++) s1[i] = tolower(s1[i]);
    for (int i = 0; s2[i]; i++) s2[i] = tolower(s2[i]);

    return strstr(s1, s2) != NULL;
}

/* ================= ADD ================= */
void addStudent() {
    Student s, temp;
    FILE *fp = fopen(FILE_NAME, "ab+");

    if (!fp) {
        printf("❌ File error.\n");
        return;
    }

    printf("Enter Roll Number : ");
    scanf("%d", &s.roll);

    rewind(fp);
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.roll == s.roll) {
            printf("❌ Roll already exists.\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter Name        : ");
    scanf(" %[^\n]", s.name);

 do {
    printf("Enter Marks (0–100): ");
    scanf("%f", &s.marks);
} while (s.marks < 0 || s.marks > 100);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("✅ Student added successfully.\n");
}

/* ================= VIEW ================= */
void viewStudents() {
    Student s;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("⚠ No records available.\n");
        return;
    }

    printf("\n---- STUDENT RECORDS ----\n");
    while (fread(&s, sizeof(s), 1, fp)) {
        printf("Roll: %d | Name: %-15s | Marks: %6.2f | Grade: %c\n",
               s.roll, s.name, s.marks, getGrade(s.marks));
    }
    fclose(fp);
}

/* ================= UPDATE ================= */
void updateStudent() {
    Student s;
    int roll, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb+");

    if (!fp) {
        printf("⚠ No records available.\n");
        return;
    }

    printf("Enter Roll to Update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            printf("New Name  : ");
            scanf(" %[^\n]", s.name);
            printf("New Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);
    printf(found ? "✅ Record updated.\n" : "❌ Student not found.\n");
}

/* ================= DELETE ================= */
void deleteStudent() {
    Student s;
    int roll, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp) {
        printf("❌ File error.\n");
        return;
    }

    printf("Enter Roll to Delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll)
            found = 1;
        else
            fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    printf(found ? "✅ Record deleted.\n" : "❌ Student not found.\n");
}

/* ================= SEARCH ================= */
void searchStudent() {
    Student s;
    int option, roll, found = 0;
    char name[50];
    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("⚠ No records available.\n");
        return;
    }

    printf("1. Search by Roll\n2. Search by Name\nChoice: ");
    scanf("%d", &option);

    if (option == 1) {
        printf("Enter Roll: ");
        scanf("%d", &roll);

        while (fread(&s, sizeof(s), 1, fp)) {
            if (s.roll == roll) {
                printf("Found: %s | %.2f | Grade %c\n",
                       s.name, s.marks, getGrade(s.marks));
                found = 1;
                break;
            }
        }
    } else {
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        while (fread(&s, sizeof(s), 1, fp)) {
            if (containsIgnoreCase(s.name, name)) {
                printf("Roll %d | %s | %.2f | %c\n",
                       s.roll, s.name, s.marks, getGrade(s.marks));
                found = 1;
            }
        }
    }

    fclose(fp);
    if (!found) printf("❌ No match found.\n");
}

/* ================= STATISTICS ================= */
void classStatistics() {
    Student s, topper;
    FILE *fp = fopen(FILE_NAME, "rb");
    int count = 0, pass = 0, fail = 0;
    float total = 0, max = -1;

    if (!fp) {
        printf("⚠ No records available.\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp)) {
        total += s.marks;
        count++;

        if (s.marks >= PASS_MARKS) pass++;
        else fail++;

        if (s.marks > max) {
            max = s.marks;
            topper = s;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("⚠ No data found.\n");
        return;
    }

    printf("\n---- CLASS STATISTICS ----\n");
    printf("Total Students : %d\n", count);
    printf("Average Marks  : %.2f\n", total / count);
    printf("Pass           : %d\n", pass);
    printf("Fail           : %d\n", fail);
    printf("Topper         : %s (%.2f)\n", topper.name, topper.marks);
}

/* ================= SORT ================= */
void sortStudents() {
    Student arr[100];
    int n = 0;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("⚠ No records available.\n");
        return;
    }

    while (fread(&arr[n], sizeof(Student), 1, fp)) n++;
    fclose(fp);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].marks < arr[j].marks) {
                Student temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    printf("\n---- SORTED BY MARKS (DESC) ----\n");
    for (int i = 0; i < n; i++) {
        printf("%d | %s | %.2f | %c\n",
               arr[i].roll, arr[i].name, arr[i].marks, getGrade(arr[i].marks));
    }
}

/* ================= MAIN ================= */
int main() {
    int choice;

    do {
        printf("\n==============================\n");
        printf(" STUDENT RECORD SYSTEM (C)\n");
        printf("==============================\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Search Student\n");
        printf("6. Class Statistics\n");
        printf("7. Sort by Marks\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: searchStudent(); break;
            case 6: classStatistics(); break;
            case 7: sortStudents(); break;
            case 8: printf("👋 Exiting...\n"); break;
            default: printf("⚠ Invalid choice.\n");
        }
    } while (choice != 8);

    return 0;
}

