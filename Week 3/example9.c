#include <stdio.h>

int main(void){
    int a[3][4];
    printf("a: %d bytes\n", sizeof(a));
    printf("a[0]:%d bytes\n", sizeof(a[0]));
    printf("a[0][0]: %d bytes\n", sizeof(a[0][0]));
}