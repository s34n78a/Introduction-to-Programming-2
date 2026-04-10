#include <stdio.h>
#include <string.h>
int main(void)
{
    int size, letters;
    char name[10]; /*character array, string*/
    printf("Hi! What's your name? ");
    scanf("%9s", name);
    printf("Your name is %s ", name);
    size = sizeof(name);
    letters = strlen(name);
    printf("size=%d, leng=%d\n", size, letters);
    printf("\n");
    char name2[10]; /*character array, string*/
    printf("Hi! What's your name? ");
    scanf("%9s", name2);
    printf("Your name is %s\n", name2);
    return 0;
}