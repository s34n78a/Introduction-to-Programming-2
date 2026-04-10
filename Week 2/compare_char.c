#include <stdio.h>
int main(void) {
    int a, b;
    printf("Enter two integers:");
    scanf("%d %d", &a, &b);
    if (a > b)

        printf("%d is larger than %d.\n",a, b);

    else if (a < b)

        printf("%d is smaller than %d.\n",a, b);

    else

        printf("%d is equal to %d.\n",a, b);
    printf("\n");

    char input;
    printf("Enter a character:");
    scanf(" %c", &input);
    if (input>='A'){

        if (input<='Z'){
            printf("upper case\n");
        } else {
            printf("NOT upper case\n");
        }
    } else {
        printf("NOT upper case\n");
    }
    printf("\n");

    char input2;
    printf("Enter a character:");
    scanf(" %c", &input2);
    printf("%c\n", input2);
    if (input2>='A' && input2<='Z')
        printf("upper case\n");
    else
        printf("NOT upper case\n");
    return 0;
}