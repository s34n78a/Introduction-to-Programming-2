#include <stdio.h>

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void sort_v1(int a[], int len){
    int i;
    // stop step
    if (len == 1) return;
    // put max value to a[len-1]
    for (i=1; i<len; i++)
        if (a[i-1]> a[i])
            swap(&a[i-1], &a[i]);

    // recursive step
    sort_v1(a, len-1);
}

void bubble_sort(int a[], int len) {
    int size, i;
    //the array size shrinks from len to 1
    for (size = len; size>0; size--)
        // swap the max of a[0..size-1]
        // to a[size-1]
        for (i = 1; i < size; i++)
            if(a[i-1]>a[i])
                swap(&a[i-1],&a[i]);

}

void sort_v2(int a[], int len){

    int i;
    // stop step
    if (len == 1) return;
    // recursive step
    sort_v2(a, len-1);
    // insert a[len-1] to the "right" place
    for (i=len-1; i>0 && a[i-1]> a[i]; i--)
        swap(&a[i-1], &a[i]);
}

void insert_sort(int a[], int len) {
    int i, size;
    // size grows from 2 to len
    for (size = 2; size<=len; size ++)
        // insert a[size-1] to the "right" place
        for(i=size-1; i>0 && a[i-1]>a[i]; i--)
            swap(&a[i], &a[i-1]);
}

int main(void){
    int a[5] = {3, 1, 4, 1, 5};
    sort_v1(a, 5);
    for(int i=0; i<5; i++)
        printf("%d ", a[i]);
    printf("\n");
    sort_v2(a, 5);
    for(int i=0; i<5; i++)
        printf("%d ", a[i]);
    printf("\n");
    insert_sort(a, 5);
    for(int i=0; i<5; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}