// week5_task4_struct_binary_io.c   (BONUS)
// Name: Victoria  Surname: Delong  Student ID: 260AMB053
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

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
int  save_student_binary(const Student *s, const char *filename);  // 0 on success, -1 on error
int  load_student_binary(const char *filename, Student *out);      // 0 on success, -1 on error
long file_size(const char *filename);                              // bytes, -1 on error

int main(void) {
    Student s1 = {0};          // zero-initialised so unused name bytes are not garbage on disk
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *bin_file = "student.bin";
    const char *txt_file = "student.txt";

    printf("Saving student in binary format...\n");
    // TODO: save_student_binary(&s1, bin_file); handle failure
    if (save_student_binary(&s1, bin_file) == -1) {
        fprintf(stderr, "Error: Could not save binary file.\n");
        return EXIT_FAILURE;
    }
    
    // TODO: Save the same record as text ("name age gpa") to txt_file for comparison
    FILE *fp = fopen(txt_file, "w");
    if (fp==NULL) {
        fprintf(stderr, "Error: Could not open text file.\n");
        return EXIT_FAILURE;
    }
    if (fprintf(fp, "%s %d %.2f", s1.name, s1.age, s1.gpa) < 0) {
        fprintf(stderr, "Error: Could not write to text file.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    if (fclose(fp)==EOF) {
        fprintf(stderr, "Error: Could not close text file.\n");
        return EXIT_FAILURE;
    }
    printf("Loading student from binary file...\n");
    
    // TODO: load_student_binary(bin_file, &loaded); handle failure
    printf("Loading student from binary file...\n");
    if (load_student_binary(bin_file, &loaded) == -1) {
        fprintf(stderr, "Error: Could not load binary file.\n");
        return EXIT_FAILURE;
    }
    
    // TODO: printf("Loaded student: %s, %d, GPA %.2f\n", ...);
    printf("Loaded student: %s, %d, GPA %.2f\n",
        loaded.name, loaded.age, loaded.gpa);
    
    // TODO: printf("Text file size: %ld bytes\n", file_size(txt_file));
    // TODO: printf("Binary file size: %ld bytes\n", file_size(bin_file));
    long txt_size = file_size(txt_file);
    long bin_size = file_size(bin_file);
    if (txt_size==-1 || bin_size==-1) {
        fprintf(stderr, "Error: Could not determine file size.\n");
        return EXIT_FAILURE;
    }
    printf("Text file size: %ld bytes\n", txt_size);
    printf("Binary file size: %ld bytes\n", bin_size);
    return EXIT_SUCCESS;
}

// TODO: open with "wb", fwrite(s, sizeof(Student), 1, fp) must return 1, close, return 0/-1
int save_student_binary(const Student *s, const char *filename) {
    if (s==NULL || filename==NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "wb");
    if (fp==NULL) {
        return -1;
    }
    size_t written = fwrite(s, sizeof(Student), 1, fp);
    int close_result = fclose(fp);
    if (written!=1 || close_result==EOF) {
        return -1;
    }
    return 0;
}

// TODO: open with "rb", fread(out, sizeof(Student), 1, fp) must return 1, close, return 0/-1
int load_student_binary(const char *filename, Student *out) {
    if (filename==NULL || out==NULL) {
        return -1;
    }
    FILE *fp = fopen(filename, "rb");
    if (fp==NULL) {
    return -1;
}
    size_t read_count = fread(out, sizeof(Student), 1, fp);
    int close_result = fclose(fp);
    if (read_count!=1 || close_result==EOF) {
        return -1;
    }
    return 0;
}

// TODO: open with "rb", fseek(fp, 0, SEEK_END), size = ftell(fp), close, return size (-1 on error)
long file_size(const char *filename) {
    if (filename==NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "rb");

    if (fp==NULL) {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END)!=0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);

    if (size==-1L) {
        fclose(fp);
        return -1;
    }

    if (fclose(fp)==EOF) {
        return -1;
    }

    return size;
}


