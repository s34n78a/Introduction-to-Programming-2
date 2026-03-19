#include <stdio.h>

int main(void){
    int row, col, current_value;
    int total = 1;
    int old_total = 0;
    scanf("%d %d", &row, &col);
    //printf("size: %dx%d\n", row, col);
    
    int arr[row][col];
    int water[row][col];
    //printf("Declared array: %p\n", (void*)arr);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            //printf("coord: %d %d\n", i, j);
            scanf("%d", &arr[i][j]);
            water[i][j] = 0;
        }
        //printf("");
    }
    water[0][0] = 1;

    // for (size_t i = 0; i < row; i++)
    // {
    //     for (size_t j = 0; j < col; j++)
    //     {
    //         printf("%d ", arr[i][j]);
    //     }
    //     printf("\n");
    // }

    while (total != old_total)
    {
        old_total = total;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                current_value = arr[i][j];
                // printf("coord: %d %d\n", i, j);
                // printf("current value: %d\n", current_value);
                // printf("current water: %d\n", water[i][j]);
                // printf("up: %d\n", (i-1>=0) ? arr[i-1][j] : 999);
                // printf("left: %d\n", (j-1>=0) ? arr[i][j-1] : 999);
                // printf("right: %d\n", (i+1<row) ? arr[i+1][j] : 999);
                // printf("down: %d\n", (j+1<col) ? arr[i][j+1] : 999);
                if (water[i][j] == 1)
                {
                    //printf("check water\n");
                    if (i > 0 && arr[i-1][j] <= current_value && arr[i-1][j] != 0 && water[i-1][j] != 1)
                    {
                        water[i-1][j] = 1;
                        total = total + 1;
                        //printf("up flow\n");
                    }
                    if (j > 0 && arr[i][j-1] <= current_value && arr[i][j-1] != 0 && water[i][j-1] != 1)
                    {
                        water[i][j-1] = 1;
                        total = total + 1;
                        //printf("left flow\n");
                    }
                    if (i+1<row && arr[i+1][j] <= current_value && arr[i+1][j] != 0 && water[i+1][j] != 1)
                    {
                        water[i+1][j] = 1;
                        total = total + 1;
                        //printf("down flow\n");
                    }
                    if (j+1<col && arr[i][j+1] <= current_value && arr[i][j+1] != 0 && water[i][j+1] != 1)
                    {
                        water[i][j+1] = 1;
                        total = total + 1;
                        //printf("right flow\n");
                    }
                }
                //printf("total: %d\n", total);
                
            }
            //printf("");
        }
    }
    
    
    printf("%d\n", total);
    
    return 0;
}