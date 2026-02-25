#include <stdio.h>

int func1(void); // Function prototype for func1, which returns an integer and takes no arguments
int func2(void);

int main(void)
{
    func1(); // Call func1, which will in turn call func2
    return 0;
}

int func1(void)
{
    func2(); // Call func2, which will print "Hello world!" to the console
    return 0;
}

int func2(void)
{
    printf("Hello world!\n");
    return 0;
}

// This program demonstrates the use of multiple functions. The main function calls func1, which then calls func2. The func2 function is responsible for printing "Hello world!" to the console. Each function returns an integer value, which is a common convention in C programming, even if the return value is not used.
// This program use so many functions because it is a simple demonstration of how functions can call each other. In more complex programs, functions are used to break down tasks into smaller, more manageable pieces, which can improve readability and maintainability of the code.