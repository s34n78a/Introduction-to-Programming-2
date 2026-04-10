#include <stdio.h>
int main(void)
/*This program shows the use character array.*/
{

    char a[2] = {'Y', 'N'};
    /* We can also use
    char a[2] = "YN";
    to initialize an array of charaters.*/
    printf("%c means Yes and %c means No\n", a[0], a[1]);

    return 0;

}