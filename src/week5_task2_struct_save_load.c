// week5_task2_struct_save_load.c
// Name: Victoria  Surname: Delong  Student ID: 260AMB053
//
// Task 2: Save and load structured records from a file
// Week 5 – Files & Modular Programming
// TODO: Complete the function implementations and the file handling logic.
//
// Build: make lab5   Run: ./bin/week5_task2_struct_save_load

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int save_student(const Student *s, const char *filename);  // 0 on success, -1 on error
int load_student(const char *filename, Student *out);      // 0 on success, -1 on error

int main(void) {
    Student s1 = {0};
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *filename = "student.txt";

    printf("Saving student to file...\n");
    // TODO: Call save_student(&s1, filename). If it fails, print an error and return EXIT_FAILURE.
if (save_student(&s1, filename)==-1) {
    fprintf(stderr, "Error: Could not save student to file.\n");
    return EXIT_FAILURE;
}
    printf("Loading student from file...\n");
    // TODO: Call load_student(filename, &loaded). If it fails, print an error and return EXIT_FAILURE.
if (load_student(filename, &loaded)==-1) {
    fprintf(stderr, "Error: Could not load student from file.\n");
    return EXIT_FAILURE;
}
    
    // TODO: Print the loaded data in exactly this format:
    //       printf("Loaded student: %s, %d, GPA %.2f\n", ...);
printf("Loaded student: %s, %d, GPA %.2f\n",
    loaded.name, loaded.age, loaded.gpa);
    (void)filename; (void)loaded;  // remove this line once you use these variables
    return EXIT_SUCCESS;
}

// TODO: Implement save_student()
// Open the file for writing ("w"), return -1 if fopen() fails,
// write "name age gpa" on one line with fprintf(), close the file and return 0.
int save_student(const Student *s, const char *filename) {
    if (s==NULL || filename==NULL) {
    (void)s; (void)filename;  // remove this line when you implement the function
    return -1;
}
    FILE *fp = fopen(filename, "w");
    if (fp==NULL) {
        return -1;
    }
    fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);
    fclose(fp);
    return 0;
}

// TODO: Implement load_student()
// Open the file for reading ("r"), return -1 if fopen() fails,
// read the three fields with fscanf(fp, "%49s %d %f", ...) and check that it returned 3,
// close the file in every case, and return 0 on success / -1 on failure.
int load_student(const char *filename, Student *out) {
    if (filename==NULL || out==NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "r");
    if (fp==NULL) {
    return -1;
    }
    int result = fscanf(fp, "%49s %d %f",
        out->name, &out->age, &out->gpa);
    fclose(fp);

    if (result != 3) {
        return -1;
    }

    return 0;
}
