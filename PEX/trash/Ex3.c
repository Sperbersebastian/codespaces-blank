//Exercise 3.0.1 Variable scopes
// 1. Compile, link and run the example programs given in the ”Variable scope” part of the lecture.
// 2.



// Exercise 3.0.2 Math functions
//1. Write a program containing functions to calculate and return square, cube and division by two of integer and float values.
#include <stdio.h>

int squareInt(int num) {
    return num * num;
}

float squareFloat(float num) {
    return num * num;
}

int cubeInt(int num) {
    return num * num * num;
}

float cubeFloat(float num) {
    return num * num * num;
}

int divideByTwoInt(int num) {
    return num / 2;
}

float divideByTwoFloat(float num) {
    return num / 2;
}

int printTask2() {
    int intNum = 5;
    float floatNum = 2.5;

    printf("Square of %d is %d\n", intNum, squareInt(intNum));
    printf("Square of %.2f is %.2f\n", floatNum, squareFloat(floatNum));

    printf("Cube of %d is %d\n", intNum, cubeInt(intNum));
    printf("Cube of %.2f is %.2f\n", floatNum, cubeFloat(floatNum));

    printf("Division by two of %d is %d\n", intNum, divideByTwoInt(intNum));
    printf("Division by two of %.2f is %.2f\n", floatNum, divideByTwoFloat(floatNum));

    return 0;
}
// 2. Write a function asking for two values to divide the first by the second one, but only if the second one is not 0.
float divideNumbers() {
    float num1, num2;
    printf("Enter the first number: ");
    scanf("%f", &num1);
    printf("Enter the second number: ");
    scanf("%f", &num2);
    
    if (num2 != 0) {
        return num1 / num2;
    } else {
        printf("Error: Division by zero is not allowed.\n");
        return 0;
    }
}
// 3. Write a program to ask for numbers (until a given stop condition), and then calculates the average.
void calculateAverage() {
    int count = 0;
    float sum = 0;
    float inputNumber;
    do {
        printf("Enter a number, for stop type 0: ");
        scanf("%f", &inputNumber);
        sum += inputNumber;
        count++;
    } while(inputNumber != 0);
    printf("Average of numbers is %.2f\n", sum / count);
}



// Exercise 3.0.3 Recursion
// 1. Write a function to calculate the factorial for a given integer. What can be the maximum value of your input integer?
unsigned long long factorial(int num) {
    if (num == 0) {
        return 1;
    } else {
        return num * factorial(num - 1);
    }
}
// 2. Write a recursive function to calculate the requested power of two.
unsigned long long powerOfTwo(int exponent) {
    if (exponent == 0) {
        return 1;
    } else {
        return 2 * powerOfTwo(exponent - 1);
    }
}

// 3. Write a recursive function to calculate the requested power of a given integer.
unsigned long long powerOfInteger(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    } else {
        return base * powerOfInteger(base, exponent - 1);
    }
}

int printAverage() {
    calculateAverage();

    // Additional example cases for recursion
    printf("Factorial of 5 is %llu\n", factorial(5));
    printf("Power of 2 with exponent 3 is %llu\n", powerOfTwo(3));
    printf("Power of 3 with exponent 4 is %llu\n", powerOfInteger(3, 4));

    return 0;
}

// 4. Give some more example cases where recursion can be used.
// Example 1: Fibonacci sequence
unsigned long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

// Example 2: Binary search
int binarySearch(int arr[], int low, int high, int target) {
    if (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] > target) {
            return binarySearch(arr, low, mid - 1, target);
        } else {
            return binarySearch(arr, mid + 1, high, target);
        }
    }
    return -1;
}

// Example 3: Tower of Hanoi
void towerOfHanoi(int n, char source, char auxiliary, char destination) {
    if (n == 1) {
        printf("Move disk 1 from %c to %c\n", source, destination);
        return;
    }
    towerOfHanoi(n - 1, source, destination, auxiliary);
    printf("Move disk %d from %c to %c\n", n, source, destination);
    towerOfHanoi(n - 1, auxiliary, source, destination);
}

//Exercise 3.0.4 Multiple source files
//1. Put your square, cube and division by two functions in a separete source code file. Rewrite your programe to use them in that way.
// 2. Write a function giving back whether a letter is lower or uppercase in a separate source 
// code file. Write a program using that function asking for ten letters one after another and
// then giving you the count of upper and lower case letters.
// 3. How to make you executable using an already compiled version of your function?

int main() {
    factorial(32767);
    return 0;
}