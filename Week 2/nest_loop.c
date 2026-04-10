#include <stdio.h>
int main(void)
{

    int i, j;
    i = 1;
    while (i<=9) {

        printf("i=%d: ", i);
        j = 1;
        while (j<=9) {

            printf("%3d ", i*j);
            j = j + 1;

        }
        printf("\n");
        i = i + 1;

    }
    return 0;

}