#include<stdio.h>
int main(void){
    int a[][4]={{1, 2, 3, 4}, {5,6},{9,10,11,12}};
    int b[][4] = { 1, 2, 3, 4, 5, 6, 9, 10, 11, 12};

    int i, j;
    printf("a: %d bytes\n", sizeof(a));
    printf("a[0]:%d bytes\n", sizeof(a[0]));
    printf("a[0][0]: %d bytes\n", sizeof(a[0][0]));
    return 0;
}