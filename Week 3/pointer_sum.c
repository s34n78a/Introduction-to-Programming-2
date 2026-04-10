#include <stdio.h>
#define SIZE 10
int sum_p(int *, int *);
int main(void){
    int arr[SIZE] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    int ans;
    ans = sum_p(arr, arr + SIZE);
    printf("The sum of the series is %d.\n", ans);
    return 0;
}
int sum_p(int *start, int *end){
    int total = 0;
    while (start < end) {
        total += *start;
        start++;
    }
    return total;
}