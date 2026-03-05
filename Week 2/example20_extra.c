#include <stdio.h>
int main()
{

    int i = 'A';
    do {

        if (i == 'C')
            continue;
        printf("%c",i);
    } while (++i < 'F');
    printf("\n");

    i = 'A';
    do {

        if (i == 'C')
            break;

        printf("%c",i);
    } while (++i < 'F');
    printf("\n");
    return 0;

}