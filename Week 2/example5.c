#include <stdio.h>
int main(void)
{

    int a[4] = {0}; //4 integers in an array
    a[0] = 3;
    a[1] = 6;
    a[2] = 9;
    a[3] = 30;
    /* Above 5 lines can be replaced by
    int a[4]={3, 6, 9, 30}; */

    printf("%d,%d,%d,%d\n", a[0], a[1], a[2], a[3]);

    return 0;

}