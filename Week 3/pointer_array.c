#include <stdio.h>
void show_int_array(int [], int);
int main(void){
    int arr[4] = {100, 200, 300, 400};
    int *ptr1, *ptr2;
    ptr1 = arr;
    ptr2 = &arr[3];
    printf("&ptr1:%p, ptr1:%p, *ptr1:%d\n", &ptr1, ptr1, *ptr1);
    printf("&ptr2:%p, ptr2:%p, *ptr2:%d\n\n", &ptr2, ptr2, *ptr2);
    show_int_array(arr, 4);
    ptr1++; *ptr1 = 201;
    ptr2--; *ptr2 = 301;
    printf("\nAfter ptr1++, ptr2--, ");
    printf("*ptr1 = 201, *ptr2 = 301\n");
    printf("&ptr1:%p, ptr1:%p, *ptr1:%d\n", &ptr1, ptr1, *ptr1);
    printf("&ptr2:%p, ptr2:%p, *ptr2:%d\n\n", &ptr2, ptr2, *ptr2);
    show_int_array(arr, 4);
    return 0;
}

void show_int_array(int a[], int n){
    int *ip;
    for (ip = a; ip < (a+n); ip++) printf("| %p ", ip);
    printf(" |\n");
    for (ip = a; ip < (a+n); ip++) printf("| %8d ", *ip);
    printf(" |\n");
}