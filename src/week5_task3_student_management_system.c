// week5_task3_student_management_system.c
// Name: Victoria  Surname: Delong  Student ID: 260AMB053
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
//
// Build: make lab5
// Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures)
int load_students(Student arr[], int max);
int save_students(const Student arr[], int count);
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    // Load existing student records from the file.
    count = load_students(students, MAX_STUDENTS);

    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;

        if (read_line("Select an option: ", input, sizeof input) != 0) {
            // End of input: save and exit instead of looping forever.
            choice = 3;
        } else {
            char *end;
            long value = strtol(input, &end, 10);

            choice = (end != input && *end == '\0')
                     ? (int)value : 0;
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;

            case 2:
                add_student(students, &count);
                break;

            case 3:
                if (save_students(students, count) == -1) {
                    printf("Error: Could not save student data.\n");
                } else {
                    printf("Data saved. Goodbye!\n");
                }
                running = 0;
                break;

            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Reads one line from stdin and removes the trailing newline.
// Returns 0 on success and -1 on end of input.
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }

    size_t len = strcspn(buf, "\r\n");

    if (buf[len] == '\0' && len == size - 1) {
        // The line may be longer than the buffer.
        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }

    buf[len] = '\0';
    return 0;
}

// Loads student records from DATA_FILE.
// Malformed lines are skipped, and no more than max records are loaded.
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        // A missing file is normal on the first run.
        return 0;
    }

    char line[LINE_LEN];
    int count = 0;

    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        Student student;
        char extra;

        // Skip blank lines and accept only records with three fields.
        if (sscanf(line, "%49[^,],%d,%f %c",
                   student.name,
                   &student.id,
                   &student.gpa,
                   &extra) == 3) {

            // Reject invalid names and GPA values.
            if (student.name[0] == '\0' ||
                strchr(student.name, ',') != NULL ||
                student.gpa < 0.0f ||
                student.gpa > 4.0f) {
                continue;
            }

            // Remove surrounding whitespace from the name.
            size_t len = strlen(student.name);

            while (len > 0 &&
                   isspace((unsigned char)student.name[len - 1])) {
                student.name[--len] = '\0';
            }

            if (student.name[0] == '\0') {
                continue;
            }

            arr[count++] = student;
        }
    }

    fclose(fp);
    return count;
}

// Saves all student records to DATA_FILE.
// Returns 0 on success and -1 on error.
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n",
                    arr[i].name,
                    arr[i].id,
                    arr[i].gpa) < 0) {
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) == EOF) {
        return -1;
    }

    return 0;
}

// Adds a new student after validating all input fields.
void add_student(Student arr[], int *count) {
    char name[LINE_LEN];
    char input[LINE_LEN];
    char *end;

    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    // Read the student's name.
    if (read_line("Enter name: ", name, sizeof name) != 0) {
        return;
    }

    // Reject empty names, commas, and names that are too long.
    if (name[0] == '\0' ||
        strchr(name, ',') != NULL ||
        strlen(name) >= NAME_LEN) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Read and validate the student ID.
    if (read_line("Enter ID: ", input, sizeof input) != 0) {
        return;
    }

    errno = 0;
    long id = strtol(input, &end, 10);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (input == end || *end != '\0' ||
        errno == ERANGE || id < INT_MIN || id > INT_MAX) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Read and validate the student's GPA.
    if (read_line("Enter GPA: ", input, sizeof input) != 0) {
        return;
    }

    errno = 0;
    float gpa = strtof(input, &end);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (input == end || *end != '\0' ||
        errno == ERANGE ||
        gpa < 0.0f || gpa > 4.0f) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // Store the new student and update the record count.
    strcpy(arr[*count].name, name);
    arr[*count].id = (int)id;
    arr[*count].gpa = gpa;

    (*count)++;

    printf("Student added successfully!\n");
}

// Displays all student records.
void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n",
               arr[i].id,
               arr[i].name,
               arr[i].gpa);
    }
}
