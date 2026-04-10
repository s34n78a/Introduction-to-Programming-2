#include<stdio.h>
int main(void)
{

    int a = 3;
    int b;
    b = a++; // b is assigned the value of a, then a is incremented by 1
    printf("a++ = %d, a = %d\n", b, a);
    b = ++a; // a is incremented by 1, then b is assigned the value of a
    printf("++a = %d, a = %d\n", b, a);
    return 0;

}