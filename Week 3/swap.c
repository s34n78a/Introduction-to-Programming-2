#include <stdio.h>

void swap_v1(int, int);
void swap_v2(int *, int *);

int main(void)
{
    int a = 2, b = 5;
    printf("Before swap_v1(), a = %d and b = %d\n", a, b);
    swap_v1(a, b);
    printf("After swap_v1(), a = %d and b = %d\n", a, b);
    printf("Before swap_v2(), a = %d and b = %d\n", a, b);
    swap_v2(&a, &b);
    printf("After swap_v2(), a = %d and b = %d\n", a, b);
    return 0;
}

void swap_v1(int u, int v){
    int temp;
    temp = u;
    u = v;
    v = temp;
}

void swap_v2(int *u, int *v){
    int temp;
    temp = *u;
    *u = *v;
    *v = temp;
}