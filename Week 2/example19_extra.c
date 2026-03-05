#include <stdio.h>
int main(void){
    int y, x;
    printf("Input a number\n");
    scanf("%d", &y);
    if (y < 0)
        x = -y;

    else

        x = y;
    printf("The absolute value is %d\n", x);
    printf("\n");

    printf("Input a number\n");
    scanf("%d", &y);
    x=(y<0)?-y:y;
    printf("The absolute value is %d\n", x);
    printf("\n");

    scanf("%d", &x);
    scanf("%d", &y);
    printf("max is %d\n", (x>y)?x:y);
    printf("\n");
    
    if (x>y)
        printf("max is %d\n", x);

    else

        printf("max is %d\n", y);
    return 0;
}