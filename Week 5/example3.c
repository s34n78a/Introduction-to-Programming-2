# include <stdio.h>
# include <string.h>
#define SIZE 20

void sswap(char s1[], char s2[]){
    char temp[SIZE];
    strcpy(temp, s1); // temp = s1
    strcpy(s1, s2); // s1 = s2
    strcpy(s2, temp); // s2 = temp
}

void swap(char **sp1, char **sp2){
    char *temp;
    temp = *sp1; // temp = s1
    *sp1 = *sp2; // s1 = s2
    *sp2 = temp; // s2 = temp
}

int main(void){
    char str1[SIZE] = "Hello";
    char str2[SIZE] = "World";
    char *pstr1 = str1;
    char *pstr2 = str2;

    printf("Before swap:\n");
    printf("str1: %s, str2: %s\n", str1, str2);
    printf("pstr1: %s, pstr2: %s\n", pstr1, pstr2);

    sswap(str1, str2);
    printf("\nAfter sswap:\n");
    printf("str1: %s, str2: %s\n", str1, str2);
    printf("pstr1: %s, pstr2: %s\n", pstr1, pstr2);

    swap(&pstr1, &pstr2);
    printf("\nAfter swap:\n");
    printf("str1: %s, str2: %s\n", str1, str2);
    printf("pstr1: %s, pstr2: %s\n", pstr1, pstr2);

    return 0;
}