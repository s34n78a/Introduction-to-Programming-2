#include <stdio.h>
float squaref(float y)
/*This function squares of a float. */
{

    return y*y;

}

double squared(double y)
/*This function squares a double. */
{

    return y*y;

}

int main(void)
/* This programs compares the
difference between float and double.*/
{

    float x = 4.3;
    double y = 4.3;
    printf("%f square = %f\n", x, squaref(x));

    printf("%lf square = %lf\n", y, squared(y));

    return 0;

}