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
    return sqrt(num);
}

// 2. Use those functions to calculate square and square root for integers from 0 to 1000 and write the output into a file.
int main() {
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


// 4. Modify the program to ask the user whether to create/overwrite or append the given file.