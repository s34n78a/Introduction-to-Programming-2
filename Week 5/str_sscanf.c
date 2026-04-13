#include <stdio.h>
#include <string.h>

int main() {
    char temp[100];
    sscanf("This is a book", "%s", temp);
    printf("%s\n", temp);

    char text[] = "This is a book";
    char *a = text;
    if (sscanf(a, "%s", temp) == 1)
    {
        a = a + strlen(temp) + 1; // Move the pointer to the next word
        printf("%s\n", temp);
    }
    
    return 0;
}