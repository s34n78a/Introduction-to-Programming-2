#include <stdio.h>
int main(void)
{

    char ch;
    for (ch = 'a'; ch <= 'z'; ch++) {
        printf("The ASCII value for %c is %d.\n", ch, ch);

    }
    printf("\n");

    int x, n;
    int a[50];
    int i = 0;
    while (i < 50) {
        printf("Input a[%d]", i);

        scanf("%d", &x);
        printf("a[%d]=%d\n", i, x);
        a[i] = x;
        i++;
    }
    printf("\n");

    for (i =0; i<50; i++) {
        printf("Input a[%d]", i);

        scanf("%d", &x);
        printf("a[%d]=%d\n", i, x);

        a[i] = x;
    }
    return 0;

}