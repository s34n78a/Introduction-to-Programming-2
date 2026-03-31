#include <stdio.h>
#include <string.h>

int main(void) {
    int t, a, b, gcd;

    scanf("%d", &t); // Read the number of test cases
    for (int i = 0; i < t; i++)
    {
        scanf("%d %d", &a, &b);
        if (a == b)
        {
            printf("%d\n",a);
            continue;
        }
        //printf("enter loop");
        while (a != 0)
        {
            
            //printf("%d %d\n", a, b);
            if (a < b)
            {
                int temp = a;
                a = b;
                b = temp;
            }
            a = a%b;
        }
        
        printf("%d\n", b);
    }
    
}