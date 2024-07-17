// 1. Write a program having three functions to calculate the square, the cube and the square root of a given number.
#include <stdio.h>
#include <math.h>

double square(double num) {
    return num * num;
}

double cube(double num) {
    return num * num * num;
}

double squareRoot(double num) {
    if (num < 0) {
        printf("Error: Negative input for square root.\n");
        return -1;
    }
    return sqrt(num);
}

// 2. Use those functions to calculate square and square root for integers from 0 to 1000 and write the output into a file.
int task2() {
    double number;
    printf("Enter a number: ");
    scanf("%lf", &number);
    
    double numSquare = square(number);
    double numCube = cube(number);
    double numSquareRoot = squareRoot(number);
    
    printf("Square: %.2lf\n", numSquare);
    printf("Cube: %.2lf\n", numCube);
    printf("Square Root: %.2lf\n", numSquareRoot);
    
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    for (int i = 0; i <= 1000; i++) {
        double numSquare = square(i);
        double numSquareRoot = squareRoot(i);
        
        fprintf(file, "Number: %d, Square: %.2lf, Square Root: %.2lf\n", i, numSquare, numSquareRoot);
    }
    
    fclose(file);
    
    return 0;
}


// 3. Modify the program to take user input numbers until a given stop condition. Write the numbers, squares, cubes and square roots to a user defined file.

#include <stdio.h>
#include <math.h>


int task3() {
    double number;
    char filename[100];
    printf("Enter the output filename: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Enter numbers to process (enter -1 to stop):\n");
    while (1) {
        printf("Enter a number: ");
        if (scanf("%lf", &number) != 1) {
            printf("Invalid input.\n");
            break;
        }

        if (number == -1) {
            break;
        }

        double numSquare = square(number);
        double numCube = cube(number);
        double numSquareRoot = squareRoot(number);

        if (numSquareRoot != -1) {
            fprintf(file, "Number: %.2lf, Square: %.2lf, Cube: %.2lf, Square Root: %.2lf\n", number, numSquare, numCube, numSquareRoot);
        } else {
            fprintf(file, "Number: %.2lf, Square: %.2lf, Cube: %.2lf, Square Root: undefined\n", number, numSquare, numCube);
        }
    }

    fclose(file);
    printf("Results written to %s\n", filename);

    return 0;
}

// 4. Modify the program to ask the user whether to create/overwrite or append the given file.

int task4() {
    double number;
    char filename[100];
    char mode;
    
    printf("Enter the output filename: ");
    scanf("%s", filename);

    printf("Do you want to (o)verwrite or (a)ppend to the file? ");
    scanf(" %c", &mode);

    FILE *file;
    if (mode == 'o' || mode == 'O') {
        file = fopen(filename, "w");
    } else if (mode == 'a' || mode == 'A') {
        file = fopen(filename, "a");
    } else {
        printf("Invalid mode.\n");
        return 1;
    }

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Enter numbers to process (enter -1 to stop):\n");
    while (1) {
        printf("Enter a number: ");
        if (scanf("%lf", &number) != 1) {
            printf("Invalid input.\n");
            break;
        }

        if (number == -1) {
            break;
        }

        double numSquare = square(number);
        double numCube = cube(number);
        double numSquareRoot = squareRoot(number);

        if (numSquareRoot != -1) {
            fprintf(file, "Number: %.2lf, Square: %.2lf, Cube: %.2lf, Square Root: %.2lf\n", number, numSquare, numCube, numSquareRoot);
        } else {
            fprintf(file, "Number: %.2lf, Square: %.2lf, Cube: %.2lf, Square Root: undefined\n", number, numSquare, numCube);
        }
    }

    fclose(file);
    printf("Results written to %s\n", filename);

    return 0;
}



int main() {

    task3();    
    return 0;
}