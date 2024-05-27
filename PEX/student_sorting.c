#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

typedef struct Student {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    float grade;
    struct Student* next;
} Student;

Student* createStudent(char* firstName, char* lastName, float grade) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed\n");
        exit(1);
    }
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

    Student dummy;
    Student* current = &dummy;
    dummy.next = NULL;

    while (left && right) {
        if (cmp(left, right) <= 0) {
            current->next = left;
            left = left->next;
        } else {
            current->next = right;
            right = right->next;
        }
        current = current->next;
    }

    current->next = (left) ? left : right;
    return dummy.next;
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
    if (a->grade < b->grade) return -1;
    if (a->grade > b->grade) return 1;
    return 0;
}

void readCSVAndCreateList(Student** studentList, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    char line[128];
    fgets(line, sizeof(line), file);  // Skip header line
    while (fgets(line, sizeof(line), file)) {
        char firstName[MAX_NAME_LENGTH];
        char lastName[MAX_NAME_LENGTH];
        float grade;
        sscanf(line, "%49[^,],%49[^,],%f", firstName, lastName, &grade);
        appendStudent(studentList, firstName, lastName, grade);
    }
    fclose(file);
}

int main() {
    Student* studentList = NULL;

    // Read data from CSV and create linked list
    readCSVAndCreateList(&studentList, "students.csv");

    // Sort by family name
    Student* sortedByLastName = mergeSort(studentList, compareByLastName);
    printToFile(sortedByLastName, "sorted_by_last_name.txt");

    // Sort by grade
    Student* sortedByGrade = mergeSort(studentList, compareByGrade);
    printToFile(sortedByGrade, "sorted_by_grade.txt");

    // Free allocated memory
    freeList(sortedByLastName);
    freeList(sortedByGrade);

    return 0;
}
