#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
void generateRandomArray(int** array, int rows, int cols);
void writeArrayToTextFile(int** array, int rows, int cols, const char* filename);
void readArrayFromTextFile(int** array, int rows, int cols, const char* filename);
void writeArrayToBinaryFile(int** array, int rows, int cols, const char* filename);
void readArrayFromBinaryFile(int** array, int rows, int cols, const char* filename);

int main() {
    int rows, cols;

    // Prompt user for the size of the array
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    // Allocate memory for the array
    int** array = (int**)malloc(rows * sizeof(int*));
    int** readArray = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        array[i] = (int*)malloc(cols * sizeof(int));
        readArray[i] = (int*)malloc(cols * sizeof(int));
    }

    generateRandomArray(array, rows, cols);

    // Text file operations
    writeArrayToTextFile(array, rows, cols, "array.txt");
    readArrayFromTextFile(readArray, rows, cols, "array.txt");

    // Verify correctness for text file
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (array[i][j] != readArray[i][j]) {
                printf("Mismatch at %d, %d for text file.\n", i, j);
                return 1;
            }
        }
    }
    printf("Text file read/write successful.\n");

    // Binary file operations
    writeArrayToBinaryFile(array, rows, cols, "array.bin");
    readArrayFromBinaryFile(readArray, rows, cols, "array.bin");

    // Verify correctness for binary file
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (array[i][j] != readArray[i][j]) {
                printf("Mismatch at %d, %d for binary file.\n", i, j);
                return 1;
            }
        }
    }
    printf("Binary file read/write successful.\n");

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(array[i]);
        free(readArray[i]);
    }
    free(array);
    free(readArray);

    return 0;
}

void generateRandomArray(int** array, int rows, int cols) {
    srand(time(NULL));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            array[i][j] = rand() % 1000;  // Random integers between 0 and 999
        }
    }
}

void writeArrayToTextFile(int** array, int rows, int cols, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", array[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void readArrayFromTextFile(int** array, int rows, int cols, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &array[i][j]);
        }
    }
    fclose(file);
}

void writeArrayToBinaryFile(int** array, int rows, int cols, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        fwrite(array[i], sizeof(int), cols, file);
    }
    fclose(file);
}

void readArrayFromBinaryFile(int** array, int rows, int cols, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        fread(array[i], sizeof(int), cols, file);
    }
    fclose(file);
}
