#include <stdio.h>
#define ROWS 3
#define COLS 4
int sum2d(int (*ap)[COLS], int rows);
/* int sum2d(int a[][COLS], int rows); */
/* int sum2d(int [][COLS], int); */
int main(void) {
    int arr2[ROWS][COLS] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    printf("Sum = %d\n", sum2d(arr2, ROWS));
    return 0;
}
int sum2d(int a[][COLS], int rows){
    int r, c;
    int total = 0;

    for (r = 0; r < rows; r++)
        for (c = 0; c < COLS; c++)
            total += a[r][c];

    return total;
}