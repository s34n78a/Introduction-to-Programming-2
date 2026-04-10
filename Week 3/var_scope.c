#include <stdio.h>
void f(int);
int main(void)
{
    int y = 2, z = 5;
    printf("In main(), y = %d and &y = %p\n", y, &y);
    printf("In main(), z = %d and &z = %p\n", z, &z);
    f(y);
    return 0;
}
void f(int z)
{
    int y = 10;
    printf("In f(), y = %d and &y = %p\n", y, &y);
    printf("In f(), z = %d and &z = %p\n", z, &z);
}