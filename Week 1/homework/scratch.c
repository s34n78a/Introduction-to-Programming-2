#include <stdio.h>

int main(void) {
    long long balance = 10000; // long long to prevent overflow because p can be up to 10^9 and there can be multiple transactions
    long long a, b, p;
    
    while (scanf("%lld %lld %lld", &a, &b, &p) == 3)
    {   
        if (balance <= 0)
        {
            printf("No balance\n");
            continue; 
        }
        
        if (a > b)
        {
            balance += p;
        }
        else if (a < b)
        {
            balance -= p;
            if (balance <= 0)
            {
                balance = 0;
                printf("%lld\n", balance);
            }
        }
    
        //printf("current balance: %lld\n", balance);
    }
    
    if (balance > 0)
    {
        printf("%lld\n", balance);
    }

    return 0;
}