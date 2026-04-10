#include <stdio.h>

int max_val1(int A[], int size);
int max_val2(int A[], int size);

int max_val1(int A[], int size){
    int max = A[0];
    for(int i=1; i<size; i++) {
        if (A[i]>max) max = A[i];
    }
    return max;
}

int max_val2(int A[], int size){
    if (size == 1) return A[0];
    int max = max_val2(A, size-1);
    return (A[size-1]>max)?A[size-1]:max;
}

int main(void){
    int A[5] = {3, 1, 4, 1, 5};
    printf("max_val1: %d\n", max_val1(A, 5));
    printf("max_val2: %d\n", max_val2(A, 5));
    return 0;
}