#include <stdio.h>

int main(void){
    int a[3][4]={1,2,3,4,5,6,7,8,9,10,11,12};
    int i, j;
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)

            printf("a[%d][%d]=%d is at %p\n", i, j, a[i][j], &a[i][j]);

}