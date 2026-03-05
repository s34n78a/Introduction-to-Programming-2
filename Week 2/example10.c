#include <stdio.h>
int main(void)
{

    int x, n;
    int i;
    int a[50];
    printf("Input n (n<50): ");
    scanf("%d", &n);
    i = 0;
    while (i<n) {

        printf("Input a[%d]: ", i);
        scanf("%d", &x);
        printf("a[%d] is %d\n", i, x);
        a[i] = x;
        i = i + 1;

    }
    return 0;

}