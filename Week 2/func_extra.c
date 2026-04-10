#include <stdio.h>
int func1(void);
int func2(void);

int square(int y)
{

    return y*y;

}
int main(void)
{

    int x;
    x = 3;
    printf("%d square is %d\n", x, square(x));
    x = 256;
    printf("%d square is %d\n", x, square(x));
    printf("\n");
    func1();
    return 0;

}
int func1(void)
{

    func2();
    return 0;

}
int func2(void)
{

    printf("Hello world!\n");
    return 0;

}