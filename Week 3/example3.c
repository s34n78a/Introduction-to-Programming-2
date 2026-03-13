#include <stdio.h>
#define SIZE 4
int main(void)
{
    int idata[SIZE] = {1, 2, 3, 4};
    int * pti; int k;
    pti = idata; /* pti = &idata[0]; */
    for (k = 0; k < 4; k++) {
        printf("%d ", idata[k]);
        printf("%d ", *(pti + k));
        printf("%d ", *(idata + k));
        printf("%d ", *(&idata[0] + k));
        printf("%d ", pti[k]);
        printf("%d ", k[pti]);
        printf("%d\n", k[idata]);
    }
    return 0;
}