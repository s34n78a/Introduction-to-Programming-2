#include <stdio.h>
#include <string.h>

int main() {

    char a[100];
    fgets(a, 99, stdin);
    a[strlen(a)-1] = '\0';

    char *token = strtok(a, " ");
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
    return 0;
}