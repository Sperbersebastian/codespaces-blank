//Exercise 3.0.1 Variable scopes
// 1. Compile, link and run the example programs given in the ”Variable scope” part of the lecture.

// local variable
# include <stdio.h>

void print_xy(int x, int y);

int main() {
    int x = 23, y = 51;
    print_xy(x, y);
    return 0;
}

void print_xy(int x, int y) {
    printf("Value of x is %d, value of y is %d.\n", x, y);
}
