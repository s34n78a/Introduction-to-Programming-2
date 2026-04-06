#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function_magic.h"

// 1. Ascending
int cmp_ascending(const void* a, const void* b){
    // typecast to Mage pointers
    const Mage *mage_a = (const Mage*) a;
    const Mage *mage_b = (const Mage*) b;

    if (mage_a->value == mage_b->value)
    {
        return strcmp(mage_a->name, mage_b->name);
    }
    else
    {
        return mage_a->value - mage_b->value;
    }
}

// 2. Square Ascending
int cmp_square_ascending(const void* a, const void* b){
    // typecast to Mage pointers
    const Mage *mage_a = (const Mage*) a;
    const Mage *mage_b = (const Mage*) b;

    if (mage_a->value*mage_a->value == mage_b->value*mage_b->value)
    {
        return strcmp(mage_a->name, mage_b->name);
    }
    else
    {
        return mage_a->value*mage_a->value - mage_b->value*mage_b->value;
    }
}

// 3. Digit Sum Ascending
int cmp_digit_sum_ascending(const void* a, const void* b){
    // typecast to Mage pointers
    const Mage *mage_a = (const Mage*) a;
    const Mage *mage_b = (const Mage*) b;

    int mage_a_sum = abs(mage_a->value/10) + abs(mage_a->value%10);
    int mage_b_sum = abs(mage_b->value/10) + abs(mage_b->value%10);

    if (mage_a_sum == mage_b_sum)
    {
        return strcmp(mage_a->name, mage_b->name);
    }
    else
    {
        return mage_a_sum - mage_b_sum;
    }
}

// 4. Swapped Digit Ascending
int cmp_swapped_digit_ascending(const void* a, const void* b){
    // typecast to Mage pointers
    const Mage *mage_a = (const Mage*) a;
    const Mage *mage_b = (const Mage*) b;

    int mage_a_digit = mage_a->value%10;
    int mage_a_tenths = mage_a->value/10;
    int mage_b_digit = mage_b->value%10;
    int mage_b_tenths = mage_b->value/10;

    int mage_a_sum = mage_a_digit*10 + mage_a_tenths;
    int mage_b_sum = mage_b_digit*10 + mage_b_tenths;
    
    if (mage_a_sum == mage_b_sum)
    {
        return strcmp(mage_a->name, mage_b->name);
    }
    else
    {
        return mage_a_sum - mage_b_sum;
    }
}

// 5. Prime First Ascending
int cmp_prime_first_ascending(const void* a, const void* b){
    // typecast to Mage pointers
    const Mage *mage_a = (const Mage*) a;
    const Mage *mage_b = (const Mage*) b;

    int mage_a_sum = mage_a->value;
    int mage_b_sum = mage_b->value;
    
    if (mage_a_sum < 0)
    {
        mage_a_sum *= -1;
    }
    
    if (mage_b_sum < 0)
    {
        mage_b_sum *= -1;
    }
    
    int mage_a_is_prime = 1;
    int mage_b_is_prime = 1;

    for (int i = 2; i*i <= mage_a_sum; i++)
    {
        if (mage_a_sum%i == 0)
        {
            mage_a_is_prime = 0;
            break;
        }
    }
    
    for (int i = 2; i*i <= mage_b_sum; i++)
    {
        if (mage_b_sum%i == 0)
        {
            mage_b_is_prime = 0;
            break;
        }
    }
    
    if (mage_a_sum == 0 || mage_a_sum == 1)
    {
        mage_a_is_prime = 0;
    }
    if (mage_b_sum == 0 || mage_b_sum == 1)
    {
        mage_b_is_prime = 0;
    }

    if (mage_a_is_prime == mage_b_is_prime)
    {
        if (mage_a->value == mage_b->value)
        {
            return strcmp(mage_a->name, mage_b->name);
        }
        
        return mage_a->value - mage_b->value;
    }
    else if (mage_a_is_prime != mage_b_is_prime)
    {
        return mage_b_is_prime - mage_a_is_prime; // if mage_a is prime, return negative to put it before mage_b
    }
}

// Use qsort with the corresponding function according to the rule.
void solve(Mage* mages, int n, int rule){
    if (rule == 0)
    {
        qsort(mages, n, sizeof(mages[0]), cmp_ascending);
    }
    else if (rule == 1)
    {
        qsort(mages, n, sizeof(mages[0]), cmp_square_ascending);
    }
    else if (rule == 2)
    {
        qsort(mages, n, sizeof(mages[0]), cmp_digit_sum_ascending);
    }
    else if (rule == 3)
    {
        qsort(mages, n, sizeof(mages[0]), cmp_swapped_digit_ascending);
    }
    else if (rule == 4)
    {
        qsort(mages, n, sizeof(mages[0]), cmp_prime_first_ascending);
    }
}
