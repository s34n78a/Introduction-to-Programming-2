#include <stdio.h>

int fun1(int x){
    printf("Inside fun1: x = %d\n", x);
    return ++x;
}

void fun2(){
    int x = 9;
    printf("Inside fun2: x = %d\n", x);
}

int main(void){
    int x = 3;
    fun1(x);
    printf("Inside main: x = %d\n", x);
    fun1(x+4);
    printf("Inside main: x = %d\n", x);
    fun2();
    printf("Inside main: x = %d\n", x);
    return 0;
}