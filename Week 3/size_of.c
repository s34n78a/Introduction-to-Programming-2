#include <stdio.h>
#define SIZE 10

int sum(int a[]){
    printf("In sum(): The size of a is %d.\n", sizeof(a));

}
int main(void){
    int arr[SIZE] = {1,1,2,3,5,8,13,21,34,55};
    sum(arr);
    printf("In main(): The size of arr is %d.\n", sizeof(arr));

    return 0;
}