#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STUDENTS 348
#define MAX_NAME_LENGTH 50
#define MEAN_GRADE 70
#define STD_DEV_GRADE 10

typedef struct Student {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    float grade;
    struct Student* next;
} Student;

Student* createStudent(char* firstName, char* lastName, float grade) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    newStudent->grade = grade;
    newStudent->next = NULL;
    return newStudent;
}

void appendStudent(Student** head, char* firstName, char* lastName, float grade) {
    Student* newStudent = createStudent(firstName, lastName, grade);
    if (*head == NULL) {
        *head = newStudent;
    } else {
        Student* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newStudent;
    }
}

void freeList(Student* head) {
    Student* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void printToFile(Student* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    fprintf(file, "%-20s %-20s %-10s\n", "First Name", "Last Name", "Grade");
    fprintf(file, "----------------------------------------------------------\n");
    Student* current = head;
    while (current != NULL) {
        fprintf(file, "%-20s %-20s %-10.2f\n", current->firstName, current->lastName, current->grade);
        current = current->next;
    }
    fclose(file);
}

Student* merge(Student* left, Student* right, int (*cmp)(Student*, Student*)) {
    if (!left) return right;
    if (!right) return left;

    if (cmp(left, right) < 0) {
        left->next = merge(left->next, right, cmp);
        left->next->next = NULL;
        return left;
    } else {
        right->next = merge(left, right->next, cmp);
        right->next->next = NULL;
        return right;
    }
}

Student* mergeSort(Student* head, int (*cmp)(Student*, Student*)) {
    if (!head || !head->next) {
        return head;
    }

    Student* slow = head;
    Student* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Student* mid = slow->next;
    slow->next = NULL;

    Student* left = mergeSort(head, cmp);
    Student* right = mergeSort(mid, cmp);

    return merge(left, right, cmp);
}

int compareByLastName(Student* a, Student* b) {
    return strcmp(a->lastName, b->lastName);
}

int compareByGrade(Student* a, Student* b) {
    return (a->grade > b->grade) - (a->grade < b->grade);
}

float generateNormalRandom(float mean, float stddev) {
    float u1 = ((float) rand() / RAND_MAX);
    float u2 = ((float) rand() / RAND_MAX);
    float z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + stddev * z;
}

void generateStudentData(Student** studentList) {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    float grade;

    for (int i = 1; i <= NUM_STUDENTS; i++) {
        sprintf(firstName, "FirstName%d", i);
        sprintf(lastName, "LastName%d", i);
        grade = generateNormalRandom(MEAN_GRADE, STD_DEV_GRADE);
        if (grade < 0) grade = 0;
        if (grade > 100) grade = 100;
        appendStudent(studentList, firstName, lastName, grade);
    }
}

int main() {
    srand(time(NULL));
    Student* studentList = NULL;

    generateStudentData(&studentList);

    // Sort by family name
    studentList = mergeSort(studentList, compareByLastName);
    printToFile(studentList, "sorted_by_last_name.txt");

    // Sort by grade
    studentList = mergeSort(studentList, compareByGrade);
    printToFile(studentList, "sorted_by_grade.txt");

    // Free allocated memory
    freeList(studentList);

    return 0;
}
