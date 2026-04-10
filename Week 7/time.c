#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    // Code to be measured goes here

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}