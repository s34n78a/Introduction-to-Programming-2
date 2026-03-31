#include <stdio.h>
#include <string.h>

int main() {

    //strstr("Hello, World!", "World") != NULL ? printf("Found\n") : printf("Not Found\n");
    if (strstr("Hello, World!", "World") != NULL)
    {
        printf("Found\n");
    }
    else
    {
        printf("Not Found\n");
    }
    
    
    return 0;
}