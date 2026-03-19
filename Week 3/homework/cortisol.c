#include <stdio.h>

int main(void){
    int size, query, a, b, total;
    scanf("%d", &size);
    int arr[size];
    //printf("The size of the array is: %d\n", size);
    //printf("Declared array: %p\n", (void*)arr);

    for (size_t i = 0; i < size; i++)
    {
        scanf("%d ",&arr[i]);
        //printf("The value of arr[%zu] is: %d\n", i, arr[i]);
    }
    
    // printf("The array is: ");
    // for (size_t i = 0; i < size; i++)
    // {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
    
    scanf("%d", &query);
    //printf("The query is: %d\n", query);

    for (size_t i = 0; i < query; i++)
    {
        scanf("%d %d", &a, &b);
        //printf("day: %d %d\n", a, b);
        
        total = 0;
        for (size_t i = a; i < b+1; i++)
        {
            total += arr[i];
            //printf("add %d\n", arr[i]);
        }
        printf("%d\n", total);
    }
    

    return 0;
}