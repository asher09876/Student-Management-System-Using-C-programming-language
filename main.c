#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float physics, chemistry, maths, biology, cs;
    float percentage;
};

float calculatePercentage(struct Student *s) {
    return (s->physics + s->chemistry + s->maths + s->biology + s->cs) / 5.0;
}

void addStudent() {
    FILE *fp = fopen("students.txt", "ab");
    if (!fp) {
        printf("Unable to open file.\n");
        return;
    }

    struct Student s;
    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter marks in Physics: ");
    scanf("%f", &s.physics);
    printf("Enter marks in Chemistry: ");
    scanf("%f", &s.chemistry);
    printf("Enter marks in Maths: ");
    scanf("%f", &s.maths);
    printf("Enter marks in Biology: ");
    scanf("%f", &s.biology);
    printf("Enter marks in Computer Science: ");
    scanf("%f", &s.cs);

    s.percentage = calculatePercentage(&s);
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
    printf("Student added successfully!\n");
}

void viewAllStudents() {
    FILE *fp = fopen("students.txt", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    struct Student s;
    printf("\n%-10s %-20s %-10s\n", "Roll No", "Name", "Percentage");

    while (fread(&s, sizeof(s), 1, fp)) {
        printf("%-10d %-20s %-10.2f\n", s.roll, s.name, s.percentage);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen("students.txt", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    struct Student s;
    int found = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            found = 1;
            printf("\n      --Student Details-- \n");
            printf("Roll No: %d\n", s.roll);
            printf("Name: %s\n", s.name);
            printf("Physics: %.2f\n", s.physics);
            printf("Chemistry: %.2f\n", s.chemistry);
            printf("Maths: %.2f\n", s.maths);
            printf("Biology: %.2f\n", s.biology);
            printf("CS: %.2f\n", s.cs);
            printf("Percentage: %.2f\n", s.percentage);
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
    }

    fclose(fp);
}

void editStudent() {
    FILE *fp = fopen("students.txt", "rb+");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int roll;
    printf("Enter roll number to edit: ");
    scanf("%d", &roll);
    struct Student s;
    int found = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            found = 1;
            fseek(fp, -sizeof(s), SEEK_CUR);
            printf("Enter new name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter new marks in Physics: ");
            scanf("%f", &s.physics);
            printf("Enter new marks in Chemistry: ");
            scanf("%f", &s.chemistry);
            printf("Enter new marks in Maths: ");
            scanf("%f", &s.maths);
            printf("Enter new marks in Biology: ");
            scanf("%f", &s.biology);
            printf("Enter new marks in Computer Science: ");
            scanf("%f", &s.cs);
            s.percentage = calculatePercentage(&s);
            fwrite(&s, sizeof(s), 1, fp);
            printf("Student record updated.\n");
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
    }

    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen("students.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");
    if (!fp || !temp) {
        printf("File error.\n");
        return;
    }

    int roll;
    printf("Enter roll number to delete: ");
    scanf("%d", &roll);
    struct Student s;
    int found = 0;

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.roll == roll) {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        printf("Student deleted.\n");
    else
        printf("Student not found.\n");
}

void sortStudents() {
    FILE *fp = fopen("students.txt", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    struct Student students[100];
    int count = 0;

    while (fread(&students[count], sizeof(struct Student), 1, fp)) {
        count++;
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].percentage < students[j + 1].percentage) {
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("\n--- Sorted Student List by Percentage ---\n");
    printf("%-10s %-20s %-10s\n", "Roll No", "Name", "Percentage");
    printf("---------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-10.2f\n", students[i].roll, students[i].name, students[i].percentage);
    }
}

int main() {
    int choice;

    do {
        printf("\n=== Student Management System ===\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Edit Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students by Percentage\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: editStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }

    } while (choice != 7);

    return 0;
}
