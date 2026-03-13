#include<stdio.h>
#define SIZE 4
int main(void)
{
    char cdata[SIZE] = {'A', 'B', 'C', 'D'};
    char * ptc;
    int idata[SIZE] = {1, 2, 3, 4};
    int * pti;
    int k;

    ptc = cdata;
    pti = idata;

    printf("%20s %12s\n", "char", "int");
    for (k = 0; k < SIZE; k++)
        printf("pointers + %d: %10p %c %10p %d\n",
            k, ptc+k, *(ptc+k), pti+k, *(pti+k));

    return 0;
}