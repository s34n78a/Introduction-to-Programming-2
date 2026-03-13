#include <stdio.h>
int main(void){
    int z[4][2] = {{1,2}, {3,4}, {5,6}, {7,8}};
    printf("|        z: %p\n", z);
    printf("|    &z[0]: %p\n", &z[0]);
    printf("|     z[0]: %p\n", z[0]);
    printf("| &z[0][0]: %p\n", &z[0][0]);
    printf("|  z[0][0]: %d\n", z[0][0]);
    printf("|      z+1: %p\n", z+1);
    printf("|    &z[1]: %p\n", &z[1]);
    printf("|   z[0]+1: %p\n", z[0]+1);
    printf("| &z[0][1]: %p\n", &z[0][1]);
    printf("|       *z: %p\n", *z);
    printf("|     z[0]: %p\n", z[0]);
    printf("|    *z[0]: %d\n", *z[0]);
    printf("|  z[0][0]: %d\n", z[0][0]);
    printf("|      **z: %d\n", **z);
    return 0;
}