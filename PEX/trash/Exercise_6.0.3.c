#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define SUCCESS_MARK 50.0

// Function to print the student list and statistics to a file
void printStudentList(FILE *file, char names[][MAX_NAME_LENGTH], char surnames[][MAX_NAME_LENGTH], double marks[], int count) {
    fprintf(file, "%-20s %-20s %-10s\n", "Name", "Surname", "Mark");
    fprintf(file, "----------------------------------------------------------\n");

    double sum = 0.0;
    double bestMark = marks[0];
    double worstMark = marks[0];
    int successfulStudents = 0;

    // Iterate through the student data to calculate sum, best mark, worst mark, and count successful students
    for (int i = 0; i < count; i++) {
        fprintf(file, "%-20s %-20s %-10.2f\n", names[i], surnames[i], marks[i]);
        sum += marks[i];
        if (marks[i] > bestMark) {
            bestMark = marks[i];
        }
        if (marks[i] < worstMark) {
            worstMark = marks[i];
        }
        if (marks[i] >= SUCCESS_MARK) {
            successfulStudents++;
        }
    }

    // Calculate average mark
    double average = sum / count;

    // Calculate median mark
    double median;
    if (count % 2 == 0) {
        median = (marks[count / 2 - 1] + marks[count / 2]) / 2.0;
    } else {
        median = marks[count / 2];
    }

    // Calculate success rate
    double successRate = (double)successfulStudents / count * 100.0;

    // Print statistics
    fprintf(file, "----------------------------------------------------------\n");
    fprintf(file, "Total Students: %d\n", count);
    fprintf(file, "Best Mark: %.2f\n", bestMark);
    fprintf(file, "Worst Mark: %.2f\n", worstMark);
    fprintf(file, "Average Mark: %.2f\n", average);
    fprintf(file, "Median Mark: %.2f\n", median);
    fprintf(file, "Number of Successful Students: %d\n", successfulStudents);
    fprintf(file, "Success Rate: %.2f%%\n", successRate);
}

// Comparison function for qsort to sort marks in ascending order
int compare(const void *a, const void *b) {
    double markA = *(double *)a;
    double markB = *(double *)b;
    if (markA < markB) return -1;
    if (markA > markB) return 1;
    return 0;
}

int main() {
    char names[MAX_STUDENTS][MAX_NAME_LENGTH];
    char surnames[MAX_STUDENTS][MAX_NAME_LENGTH];
    double marks[MAX_STUDENTS];
    int studentCount = 0;

    printf("Enter student details (name surname mark). Enter 'done' to finish:\n");

    // Loop to collect student details
    while (studentCount < MAX_STUDENTS) {
        char name[MAX_NAME_LENGTH];
        char surname[MAX_NAME_LENGTH];
        double mark;

        printf("Student %d: ", studentCount + 1);
        if (scanf("%s", name) != 1 || strcmp(name, "done") == 0) {
            break;
        }

        if (scanf("%s", surname) != 1 || strcmp(surname, "done") == 0) {
            break;
        }

        if (scanf("%lf", &mark) != 1) {
            printf("Invalid input. Try again.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }

        // Store the collected data in arrays
        strcpy(names[studentCount], name);
        strcpy(surnames[studentCount], surname);
        marks[studentCount] = mark;
        studentCount++;
    }

    // Sort the marks array to calculate the median
    qsort(marks, studentCount, sizeof(double), compare);

    char filename[100];
    printf("Enter the output filename: ");
    scanf("%s", filename);

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Print the student list and statistics to the file
    printStudentList(file, names, surnames, marks, studentCount);

    // Close the file
    fclose(file);
    printf("Results written to %s\n", filename);

    return 0;
}
