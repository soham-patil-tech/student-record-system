#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float marks;
};

char getGrade(float marks) {
    if (marks >= 75) return 'A';
    else if (marks >= 60) return 'B';
    else if (marks >= 40) return 'C';
    else return 'F';
}

/* ---------------- ADD STUDENT ---------------- */
void addStudent() {
    struct Student s, temp;
    FILE *fp = fopen("students.dat", "ab+");

    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);

    rewind(fp);
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.roll == s.roll) {
            printf("❌ Roll number already exists!\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("✅ Student added successfully!\n");
}

/* ---------------- VIEW STUDENTS ---------------- */
void viewStudents() {
    struct Student s;
    FILE *fp = fopen("students.dat", "rb");

    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(s), 1, fp)) {
        printf("Roll: %d | Name: %s | Marks: %.2f | Grade: %c\n",
               s.roll, s.name, s.marks, getGrade(s.marks));
    }
    fclose(fp);
}

/* ---------------- UPDATE STUDENT ---------------- */
void updateStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen("students.dat", "rb+");

    printf("Enter Roll Number to Update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            printf("Enter New Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (found) printf("✅ Record updated!\n");
    else printf("❌ Student not found.\n");
}

/* ---------------- SEARCH BY NAME ---------------- */
void searchByName() {
    struct Student s;
    char key[50];
    int found = 0;
    FILE *fp = fopen("students.dat", "rb");

    printf("Enter name to search: ");
    scanf(" %[^\n]", key);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (strstr(s.name, key)) {
            printf("Roll: %d | Name: %s | Marks: %.2f | Grade: %c\n",
                   s.roll, s.name, s.marks, getGrade(s.marks));
            found = 1;
        }
    }

    fclose(fp);
    if (!found) printf("❌ No matching student found.\n");
}

/* ---------------- TOTAL, AVERAGE, TOPPER ---------------- */
void classStats() {
    struct Student s, topper;
    FILE *fp = fopen("students.dat", "rb");
    int count = 0;
    float total = 0, max = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        total += s.marks;
        count++;
        if (s.marks > max) {
            max = s.marks;
            topper = s;
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("No data available.\n");
        return;
    }

    printf("\nTotal Students: %d", count);
    printf("\nAverage Marks: %.2f", total / count);
    printf("\nTopper: %s (%.2f)\n", topper.name, topper.marks);
}

/* ---------------- MAIN MENU ---------------- */
int main() {
    int choice;

    do {
        printf("\n============================\n");
        printf(" Student Record System\n");
        printf("============================\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Update Student\n");
        printf("4. Search by Name\n");
        printf("5. Class Statistics\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: searchByName(); break;
            case 5: classStats(); break;
            case 6: printf("👋 Exiting...\n"); break;
            default: printf("⚠ Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}
