#include <stdio.h>
#include <stdlib.h>
#include "function_3d.h"

unsigned*** new_3d_array(unsigned n,unsigned m,unsigned k){
    unsigned i;

    unsigned *level_1 = (unsigned*)malloc(n * m * k * sizeof(unsigned));
    unsigned **level_2 = (unsigned**)malloc(n * m * sizeof(unsigned*));
    unsigned ***level_3 = (unsigned***)malloc(n * sizeof(unsigned**));

    if (level_1 == NULL || level_2 == NULL || level_3 == NULL)
    {
        free(level_1);
        free(level_2);
        free(level_3);
        return NULL; // Allocation failed
    }
    

    for (i = 0; i < n*m; i++)
    {
        level_2[i] = level_1 + i * k; //every m*k elements are for one n
    }
    
    for (i = 0; i < n; i++)
    {
        level_3[i] = level_2 + i * m; //every m elements are for one n
    }

    return level_3;
}

void delete_3d_array(unsigned ***arr) {
    free(arr[0][0]);
    free(arr[0]);
    free(arr);
}