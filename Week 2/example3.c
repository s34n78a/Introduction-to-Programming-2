#include <stdio.h>
#define ENGINE 1499.99
int main(void)
{
    printf("~%f~\n", ENGINE);
    printf("~%e~\n", ENGINE);
    printf("~%4.2f~\n", ENGINE);
    printf("~%3.1f~\n", ENGINE);
    printf("~%10.3f~\n", ENGINE);
    printf("~%-10.3f~\n", ENGINE);
    printf("~%12.3e~\n", ENGINE);
    printf("~%+4.2f~\n", ENGINE);
    printf("~%010.2f~\n", ENGINE);
    printf("\n");
    printf("~%f~\n", 1499.99);
    printf("~%e~\n", 1499.99);
    printf("~%4.2f~\n", 1499.99);
    printf("~%3.1f~\n", 1499.99);
    printf("~%10.3f~\n", 1499.99);
    printf("~%-10.3f~\n", 1499.99);
    printf("~%12.3e~\n", 1499.99);
    printf("~%+4.2f~\n", 1499.99);
    printf("~%010.2f~\n", 1499.99);
    return 0;
}