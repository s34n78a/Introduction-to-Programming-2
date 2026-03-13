#include <stdio.h>
#include <stdlib.h>
int main() {
    int *ptr;
    ptr = (int*) malloc(5 * sizeof(int));
    for(int i = 0; i < 5; i++) {
        ptr[i] = i + 1;
        printf("%d ", ptr[i]);
    }
    free(ptr); //free memory leaks
    return 0;
}