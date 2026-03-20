#include <stdio.h>
#include <stdlib.h>
#include "function_walk.h"

int **create_walkway(int n, const int *len) {
    int **tile = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
    {
        tile[i] = (int*) malloc(len[i] * sizeof(int));
    }
    return tile;
}

void activate_walkway(int n, const int *len, int **tile) {
    int left, middle, right;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < len[i]; j++)
        {
            if (j == 0)
            {
                left = 0;
            }

            if (j == len[i] - 1)
            {
                right = 0;
            }
            else
            {
                right = tile[i][j + 1];
            }
            
            middle = tile[i][j];
            
            tile[i][j] = middle + left + right;

            left = middle;
        }
    }
}

void delete_walkway(int n, int **tile) {
    for (int i = 0; i < n; i++)
    {
        free(tile[i]);
    }
    free(tile);    
}