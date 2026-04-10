#include <stdio.h>
int main(void)
{

    int x; //integer 32-bit
    double y; // 64-bit real number
    x = 5;
    y = 5;

    // formatted output
    printf("%d, %5d, %d\n", x, x, x/2);
    // %d for decimal integer
    printf("%f, %10.5f, %f\n", y, y, y/2);
    // %f for floating number
    return 0;

}