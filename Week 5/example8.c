#include <stdio.h>
#include <string.h>

int main(void) {

    char str1[20] = "Hello";
    char str2[20] = "World";
    char str3[20];

    // Using memcpy
    memcpy(str3, str1, sizeof(str1));
    printf("Copied string: %s\n", str3);

    return 0;
}