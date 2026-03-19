#include <stdio.h>
#include "add1.h"
int add1(int x){
    x ++;
    printf("Inside add1: x = %d\n", x);
    return x;
}