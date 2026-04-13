#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 10

int compare_str(const void *a, const void *b){

    const char *pa = (const char *) a;
    const char *pb = (const char *) b;
    return strcmp(pa, pb);

}

int main(void) {
    char strs[10][4] ={"aab", "abc", "aaa", "abb", "acb", "bac", "bbc", "ccc", "cba", "bca"};
    qsort(strs, SIZE, 4*sizeof(char), compare_str);
    printf("Sorted strings:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%s\n", strs[i]);
    }

    char strs2[10][4] = {"aab", "abc", "aaa", "abb", "acb", "bac", "bbc", "ccc", "cba", "bca"};
    qsort(strs2, SIZE, 4*sizeof(char), (int (*) (const void *, const void *))strcmp);
    printf("Sorted strings:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%s\n", strs2[i]);
    }

    return 0;
}