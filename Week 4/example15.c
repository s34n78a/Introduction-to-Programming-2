#include <stdio.h>

typedef int (*MathMethod)(int, int);
int Mul(int a, int b) { return a*b; }
int Divide(int a, int b) { return a/b; }
int Minus(int a, int b) { return a-b; }
int Add(int a, int b) { return a+b; }

int *Calc(int x, int y, MathMethod Opt)
{
    return Opt(x, y);
}

int main()
{
    int a = 5, b = 10;
    printf("a x b = %d\n", Calc(a, b, Mul));
    printf("a / b = %d\n", Calc(a, b, Divide));
    printf("a + b = %d\n", Calc(a, b, Minus));
    printf("a - b = %d\n", Calc(a, b, Add));
}