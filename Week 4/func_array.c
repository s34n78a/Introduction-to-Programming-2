#include <stdio.h>

int fun(int A[3]){
    A[0] = 2; A[2] = 0;
    return 0;
}

int main(void){
    int B[3] = {10,100, 1000};

    fun(B);
    printf("%d %d %d\n", B[0], B[1], B[2]);
    return 0;
}