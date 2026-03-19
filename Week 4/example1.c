#include<stdio.h>
int add1(int x){
    x ++;
    printf("Inside add1: x = %d\n", x);
    return x;
}
int main(void){
    int x = 3, y;
    printf("Before add1: x = %d\n", x);
    y = add1(x);
    printf("After add1: x = %d\n", x);
    printf("After add1: y = %d\n", y);
    return 0;
}