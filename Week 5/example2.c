#include <stdio.h>

int main(void){
    int i, j;
    char str1[][8] = {"piece", "of", "cake"};
    char *str2[] = {"piece", "of", "cake"};

    for (i=0; i<3; i++) {
        for (j=0;j<8;j++) printf("%c", str1[i][j]);
        printf("\n");
    }
    for (i=0;i<3;i++){
        for (j=0;j<8;j++) printf("%c", str2[i][j]);
        printf("\n");
    }
}