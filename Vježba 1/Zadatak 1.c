#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50
#define MAX_POINTS 50.0  // Change this to exam's max points

// Structure to store student information
typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    float points;
} Student;

int main() {
    FILE *file;
    char filename[] = "studenti.txt"; // Name of the input file
    int num_students = 0;
    char line[MAX_LINE_LENGTH];

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    // First, count the number of lines (students)
    while (fgets(line, sizeof(line), file)) {
        num_students++;
    }

    // Go back to the beginning of the file
    rewind(file);

    // Dynamically allocate memory for students
    Student *students = (Student *)malloc(num_students * sizeof(Student));
    if (students == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    // Read student records from file
    for (int i = 0; i < num_students; i++) {
        if (fscanf(file, "%s %s %f", students[i].name, students[i].surname, &students[i].points) != 3) {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            free(students);
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    // Print student information
    printf("Name\tSurname\tPoints\tRelative Points (%%)\n");
    for (int i = 0; i < num_students; i++) {
        float relative = (students[i].points / MAX_POINTS) * 100;
        printf("%s\t%s\t%.2f\t%.2f%%\n", students[i].name, students[i].surname, students[i].points, relative);
    }

    // Free allocated memory
    free(students);

    return 0;
}
