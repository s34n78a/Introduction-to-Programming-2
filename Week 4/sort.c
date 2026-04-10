#include <stdio.h>

void sort(int a[], int len){
    int i, maxval, maxidx;
    // stop step
    if (len == 1) return;
    // find the max of a[0...length-1]
    maxval = a[0]; maxidx = 0;
    for(i=1; i<len; i++)
        if (maxval<a[i]) { maxval=a[i]; maxidx=i;}
    // swap the max with the end of a[]
    a[maxidx]=a[len-1]; a[len-1]=maxval;
    // recursive step
    sort(a, len-1);
}

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void findMax(int a[], int n){
    if (n == 1) return;
    findMax(a, n-1);
    if (a[n-1]<a[n-2]) swap(&a[n-1], &a[n-2]);
}

void sort2(int a[], int len){
    if (len == 1) return;
    // find the max of a[0...length-1]
    findMax(a, len);
    // recursive step
    sort2(a, len-1);
}

int main(void){
    int a[5] = {3, 1, 4, 1, 5};
    sort(a, 5);
    for(int i=0; i<5; i++)
        printf("%d ", a[i]);
    printf("\n");
    sort2(a, 5);
    for(int i=0; i<5; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}