// Session 2
// Data types and control constructs

// 2.1 Data types
// Exercise 2.1.1 Integer data type ranges
// 1. Write a program to get the minimum and maximum values for signed and unsigned char, short, int and long data types on your system.
#include <stdio.h>
#include <limits.h>

void printDataTypeRanges() {
    printf("Minimum Signed Char %d\n", SCHAR_MIN);
    printf("Maximum Signed Char %d\n", SCHAR_MAX);
    printf("Maximum Unsigned Char %d\n", UCHAR_MAX);

    printf("Minimum Signed Short %d\n", SHRT_MIN);
    printf("Maximum Signed Short %d\n", SHRT_MAX);
    printf("Maximum Unsigned Short %d\n", USHRT_MAX);

    printf("Minimum Signed Int %d\n", INT_MIN);
    printf("Maximum Signed Int %d\n", INT_MAX);
    printf("Maximum Unsigned Int %u\n", UINT_MAX);

    printf("Minimum Signed Long %ld\n", LONG_MIN);
    printf("Maximum Signed Long %ld\n", LONG_MAX);
    printf("Maximum Unsigned Long %lu\n", ULONG_MAX);
}

// 2. Write a program to get the size of signed and unsigned char, short, int and long data types on your system.
void printDataTypeSizes() {
    printf("Size of Signed Char %zu\n", sizeof(char));
    printf("Size of Unsigned Char %zu\n", sizeof(unsigned char));
    printf("Size of Signed Short %zu\n", sizeof(short));
    printf("Size of Unsigned Short %zu\n", sizeof(unsigned short));
    printf("Size of Signed Int %zu\n", sizeof(int));
    printf("Size of Unsigned Int %zu\n", sizeof(unsigned int));
    printf("Size of Signed Long %zu\n", sizeof(long));
    printf("Size of Unsigned Long %zu\n", sizeof(unsigned long));
}


// 3. How can you calculate the maximum value once you have the size?
// 3. How can you calculate the maximum value once you have the size?
void calculateMaxValueFromSize() {
    printf("Max Value of Signed Char %ld\n", (1L << ((sizeof(char) * 8) - 1)) - 1);
    printf("Max Value of Unsigned Char %lu\n", (1UL << (sizeof(unsigned char) * 8)) - 1);
    printf("Max Value of Signed Short %ld\n", (1L << ((sizeof(short) * 8) - 1)) - 1);
    printf("Max Value of Unsigned Short %lu\n", (1UL << (sizeof(unsigned short) * 8)) - 1);
    printf("Max Value of Signed Int %ld\n", (1L << ((sizeof(int) * 8) - 1)) - 1);
    printf("Max Value of Unsigned Int %lu\n", (1UL << (sizeof(unsigned int) * 8)) - 1);
    printf("Max Value of Signed Long %ld\n", (1L << ((sizeof(long) * 8) - 1)) - 1);
    printf("Max Value of Unsigned Long %lu\n", (1UL << (sizeof(unsigned long) * 8)) - 1);
}

// Exercise 2.1.2 Numerical storage of characters
// 1. Write a program that asks for a letter and gives back its numerical representation.
void printNumericalRepresentation() {
    char inputChar;
    printf("Enter a character: ");
    scanf("%c", &inputChar);
    printf("Numerical representation of %c is %d\n", inputChar, inputChar);
}
// 2. Modify the program to print out the letter and its numerical representation.
void printLetterAndNumericalRepresentation() {
    int inputNumber;
    printf("Enter a number: ");
    scanf("%d", &inputNumber);
    printf("Character representation of %d is %c\n", inputNumber, inputNumber);
}
// 2.2 Control constructs
// Exercise 2.2.1 Printing out numbers
// 1. Write a program to print out numbers from 1 to 100.
void printNumbers() {
    for(int i = 1; i <= 100; i++) {
        printf("%d\n", i);
    }
}
// 2. Write a program to print out numbers from 1 to 100 separating them by commas.
void printNumbersWithCommas() {
    for(int i = 1; i <= 100; i++) {
        printf("%d, ", i);
    }
}
// 3. Modify the program to print out 10 numbers per line.
void printNumbersWithCommasAndNewLine() {
    for(int i = 1; i <= 100; i++) {
        printf("%d, ", i);
        if(i % 10 == 0) {
            printf("\n");
        }
    }
}
// Exercise 2.2.2 Sum of integers
// 1. Write a program asking for integers and summing them up until you enter 0.
void sumIntegers() {
    int sum = 0;
    int inputNumber;
    do {
        printf("Enter a number: ");
        scanf("%d", &inputNumber);
        sum += inputNumber;
    } while(inputNumber != 0);
    printf("Sum of numbers is %d\n", sum);
}

// Exercise 2.2.3 Optimal data type
// 1. Write a program to calculate the optimal data type (in terms of storage efficiency) for a given integer.
void calculateOptimalDataType() {
    int inputNumber;
    printf("Enter a number: ");
    scanf("%d", &inputNumber);
    if(inputNumber >= SCHAR_MIN && inputNumber <= SCHAR_MAX) {
        printf("Optimal data type is char\n");
    } else if(inputNumber >= SHRT_MIN && inputNumber <= SHRT_MAX) {
        printf("Optimal data type is short\n");
    } else if(inputNumber >= INT_MIN && inputNumber <= INT_MAX) {
        printf("Optimal data type is int\n");
    } else {
        printf("Optimal data type is long\n");
    }
}

// Exercise 2.2.4 Multiple choice
// 1. Write a program printing a multiple choice question, asking for an answer and checking if that’s the right one or not.
void multipleChoiceQuestion() {
    char answer;
    printf("What is the capital of France?\n");
    printf("A. London\n");
    printf("B. Paris\n");
    printf("C. Berlin\n");
    printf("D. Madrid\n");
    printf("Enter your answer: ");
    scanf(" %c", &answer);
    if(answer == 'B') {
        printf("Correct answer!\n");
    } else {
        printf("Wrong answer!\n");
    }
}
// 2. Modify the program to ask for an answer until it gets the right one.
void multipleChoiceQuestionWithRetry() {
    char answer;
    do {
        printf("What is the capital of France?\n");
        printf("A. London\n");
        printf("B. Paris\n");
        printf("C. Berlin\n");
        printf("D. Madrid\n");
        printf("Enter your answer: ");
        scanf(" %c", &answer);
    } while(answer != 'B');
    printf("Correct answer!\n");
}
// 3. Extend the program to present three questions one after another, asking for answers (one try each time) and then returning the number of correct answers.
void multipleChoiceQuestions() {
    char answer;
    int correctAnswers = 0;
    printf("What is the capital of France?\n");
    printf("A. London\n");
    printf("B. Paris\n");
    printf("C. Berlin\n");
    printf("D. Madrid\n");
    printf("Enter your answer: ");
    scanf(" %c", &answer);
    if(answer == 'B') {
        correctAnswers++;
    }

    printf("What is the capital of Germany?\n");
    printf("A. London\n");
    printf("B. Paris\n");
    printf("C. Berlin\n");
    printf("D. Madrid\n");
    printf("Enter your answer: ");
    scanf(" %c", &answer);
    if(answer == 'C') {
        correctAnswers++;
    }

    printf("What is the capital of Spain?\n");
    printf("A. London\n");
    printf("B. Paris\n");
    printf("C. Berlin\n");
    printf("D. Madrid\n");
    printf("Enter your answer: ");
    scanf(" %c", &answer);
    if(answer == 'D') {
        correctAnswers++;
    }

    printf("Number of correct answers: %d\n", correctAnswers);
}


int main() {

    multipleChoiceQuestions();    
    return 0;
}